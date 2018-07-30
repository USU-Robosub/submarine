const {
  comm,
  scheduler:{ Scheduler, Command },
  subsystems:{ dive, tank, imu, pose, killswitch },
  commands:{ remoteControl, aiLauncher }
} = require('./utils')

const { ai } = require('./ai')

const settings = require("./settings")

const path = require('path')
const express = require('express')
const app = express();
const http = require('http').Server(app)
const io = require('socket.io')(http)
const { map, buffer, debounceTime, filter, tap, withLatestFrom, takeUntil } = require('rxjs/operators')
const { merge, interval } = require('rxjs')


app.get('/', (req, res) => res.sendFile(path.resolve(__dirname + '/../webapp/index.html')))
app.get('/rsapp/socket.js', (req, res) => res.sendFile(path.resolve(__dirname + '/../../extern/socketio/socket.io.min.js')))
app.get('/rsapp/three.js', (req, res) => res.sendFile(path.resolve(__dirname + '/node_modules/three/build/three.min.js')))
app.get('/rsapp/chart.min.js', (req, res) => res.sendFile(path.resolve(__dirname + '/node_modules/chart.js/dist/Chart.min.js')))
app.get('/rsapp/rx.js', (req, res) => res.sendFile(path.resolve(__dirname + '/../../extern/rxjs/rx.min.js')))
app.get('/rsapp/riot-compiler.min.js', (req, res) => res.sendFile(path.resolve(__dirname + '/../../extern/riot/riot+compiler.min.js')))
app.use(express.static(path.resolve(__dirname + '/../webapp')))
app.use(express.static(path.resolve(__dirname + '/node_modules/riotgear')))

init()

http.listen(settings.webApp.port,
  () => console.log('Web app listening on port', settings.webApp.port)
)

function echoToAll(data){
  io.local.emit('echo/agent', data)
}

const browser = {
  good(...data){
    echoToAll(['-1', ...data])
  },
  info(...data){
    echoToAll(['0', ...data])
  },
  warn(...data){
    echoToAll(['1', ...data])
  },
  error(...data){
    echoToAll(['2', ...data])
  }
}

var hasSystemFailed = false;
function init(){
  comm.create({
    address: settings.system.address,
    port: settings.system.port,
    separator: settings.system.separator
  }).then(hub => {
    try{
      startAgent(hub)
    }catch(e){
      console.log('Failed to run agent', e)
    }
  }).catch(error => {
    if(!hasSystemFailed)
      console.error('Failed to connect to system.', error)
    hasSystemFailed = true;
    setTimeout(init, settings.system.retryDelay) // try connecting again
  })
}

function startAgent(hub){
  console.log('Connected to system')

  let tankObj = tank(hub);
  let diveObj = dive(hub);

  const scheduler = Scheduler([
    diveObj.subsystem,
    tankObj.subsystem,
    imu(hub),
    pose(hub),
    killswitch(hub)
  ])

  // start subsystem internal commands to maintain state
  tankObj.defaultCommand(scheduler)
  diveObj.defaultCommand(scheduler)

  // allow ai to run
  scheduler.build(aiLauncher(ai)).then().run().to.promise().catch(e => {
    console.log(e)
    browser.error(e);
  })
  browser.info('Started AI launcher. "Double click" the kill switch to start AI script')

  // allow webapp interactions
  connectToWebApp(scheduler, hub)

  // start listening to system
  setInterval(hub.poll, settings.system.loopDelay)
}

function connectToWebApp(scheduler, hub){
  // setup clients already connected
  Object.values(io.sockets.sockets).forEach(socket => {
    setupWebAppClient(scheduler, socket)
  });

  // setup new clients
  io.on('connection', socket => {
    browser.info('Client Connected')

    hub.on('echo/arduino', (hub, data) => {
      socket.emit('echo/arduino', data)
    })

    hub.on('echo/system', (hub, data) => {
      socket.emit('echo/system', data)
    })

    hub.on('imu/data', (hub, data) => {
      //console.log(data)
      socket.emit('imu/raw_large', data.map(str => parseFloat(str)))
    })

    setupWebAppClient(scheduler, socket)
  })
}

function setupWebAppClient(scheduler, socket){
  console.log('A user connected')

  scheduler.run(
    Command()
      .named('killswitch monit')
      .require('killSwitch')
      .action(system => {
        return system.killSwitch.status().pipe(
          map(status => {
            if(status){
              socket.emit('killswitch/on')
            }else{
              socket.emit('killswitch/off')
            }
          })
        )
      })
  )

  scheduler.run(remoteControl.dive(socket)).to.promise().then(() => {
    socket.emit('dive/lost')
    console.log('Dive remote control stopped for user')
  })

  scheduler.run(remoteControl.tank(socket)).to.promise().then(() => {
    socket.emit('tank/lost')
    console.log('Tank remote control stopped for user')
  })

  scheduler.run(remoteControl.readPose(socket)).to.promise().then(() => {
    socket.emit('pose/lost')
    console.log('Pose remote control stopped for user')
  })

  socket.on('disconnect', function(){
    console.log('A user disconnected')
    // TODO cancel remote control commands
  })

  // hub.on('killswitch', function(message){
  //   console.log('kill switch', message)
  //   socket.emit(message)
  // })
}
