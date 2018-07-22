const {
  comm,
  scheduler:{ Scheduler },
  subsystems:{ dive, tank },
  commands:{ remoteControl }
} = require('./utils')

const settings = require("./settings")

const path = require('path')
const express = require('express')
const app = express();
const http = require('http').Server(app)
const io = require('socket.io')(http)

app.get('/', (req, res) => res.sendFile(path.resolve(__dirname + '/../webapp/index.html')))
app.get('/rsapp/socket.js', (req, res) => res.sendFile(path.resolve(__dirname + '/../../extern/socketio/socket.io.min.js')))
app.get('/rsapp/rx.js', (req, res) => res.sendFile(path.resolve(__dirname + '/../../extern/rxjs/rx.min.js')))
app.get('/rsapp/index.js', (req, res) => res.sendFile(path.resolve(__dirname + '/../webapp/index.js')))
app.use(express.static(path.resolve(__dirname + '/../webapp')))

init()

http.listen(settings.webApp.port,
  () => console.log('Web app listening on port', settings.webApp.port)
)

function init(){
  comm.create({
    address: settings.system.address,
    port: settings.system.port,
    separator: settings.system.separator
  }).then(hub => {
    startAgent(hub)
  }).catch(error => {
    console.log('Failed to connect to system.')
    setTimeout(init, settings.system.retryDelay) // try connecting again
  })
}

function startAgent(hub){
  console.log('Connected to system')

  const scheduler = Scheduler([
    dive(hub),
    tank(hub)
  ])

  connectToWebApp(scheduler)

  setInterval(hub.poll, settings.system.loopDelay) // start listening to system
}

function connectToWebApp(scheduler){
  // setup clients already connected
  Object.values(io.sockets.sockets).forEach(socket => {
    setupWebAppClient(scheduler, socket)
  });

  // setup new clients
  io.on('connection', socket => {
    setupWebAppClient(scheduler, socket)
  })
}

function setupWebAppClient(scheduler, socket){
  console.log('A user connected')

  scheduler.run(remoteControl.dive(socket)).to.promise().then(() => {
    socket.emit('dive/lost')
    console.log('Dive remote control stopped for user')
  })

  scheduler.run(remoteControl.tank(socket)).to.promise().then(() => {
    socket.emit('tank/lost')
    console.log('Tank remote control stopped for user')
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
