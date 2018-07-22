const { comm, scheduler:{ Subsystem, Command, Scheduler, tools } } = require('./utils')
const path = require('path')
const express = require('express')
const app = express();
const http = require('http').Server(app)
const io = require('socket.io')(http)
const { map, throttleTime } = require('rxjs/operators')
const { fromEvent, merge, Observable } = require('rxjs')

app.get('/', (req, res) => res.sendFile(path.resolve(__dirname + '/../webapp/index.html')))
app.get('/rsapp/socket.js', (req, res) => res.sendFile(path.resolve(__dirname + '/../../extern/socketio/socket.io.min.js')))
app.get('/rsapp/three.js', (req, res) => res.sendFile(path.resolve(__dirname + '/node_modules/three/build/three.min.js')))
app.get('/rsapp/rx.js', (req, res) => res.sendFile(path.resolve(__dirname + '/../../extern/rxjs/rx.min.js')))
app.get('/rsapp/index.js', (req, res) => res.sendFile(path.resolve(__dirname + '/../webapp/index.js')))
app.use(express.static(path.resolve(__dirname + '/../webapp')))
function init(){
  comm.create({
    address: 'localhost',
    port: 3001,
    separator: '|'
  }).then(hub => {

    console.log('connected to system')
    console.log('starting ai...')

    const scheduler = Scheduler([
      subsystems.dive(hub),
      subsystems.tank(hub),
      subsystems.imu(hub)
    ])

    io.on('connection', function(socket){
      console.log('a user connected')

      scheduler.run(listenToRotation(socket)).to.promise().then(() => {
        socket.emit('imu/lost')
        console.log('Imu listen stopped for user')
      }).catch(e => console.error(e))

      scheduler.run(remoteControlDive(socket)).to.promise().then(() => {
        socket.emit('dive/lost')
        console.log('Dive remote control stopped for user')
      })

      scheduler.run(remoteControlTank(socket)).to.promise().then(() => {
        socket.emit('tank/lost')
        console.log('Tank remote control stopped for user')
      })

      socket.on('disconnect', function(){
        console.log('user disconnected')
      })

      hub.on('killswitch', function(message){
        console.log('kill switch', message)
        socket.emit(message)
      })
    })

    setInterval(hub.poll, 10)
  }).catch(error => {
    console.log(error)
    setTimeout(init, 500)
  })
}

const forwardData = (hub, module, method) => amount => {
  console.log('Set', method, 'to', amount)
  hub.emit(module + '/' + method, amount)
}

const subsystems = {
  imu: hub => Subsystem()
    .named('imu')
    .makeShared()
    .raw({
      // rotation: () => fromEvent(hub, 'imu/rotation').pipe(
      //   map(([ x, y, z]) => [parseInt(x), parseInt(y), parseInt(z)])
      // ),
      rotation: () => new Observable(observer => {
        const listener = hub.on('imu/rotation', (hub, data) => {
          observer.next(data)
        })
        return () => {
          listener.remove()
        }
      })
    }),
  dive: hub => Subsystem()
    .named('dive')
    .raw({
      power: forwardData(hub, 'dive', 'power'),
    }),
  tank: hub => Subsystem()
    .named('tank')
    .raw({
      throttle: forwardData(hub, 'tank', 'throttle'),
      steering: forwardData(hub, 'tank', 'steering'),
      left: forwardData(hub, 'tank', 'left'),
      right: forwardData(hub, 'tank', 'right'),
    }),
  pid: hub => Subsystem()
    .named('pid')
    .raw({
      setpoint: forwardData(hub, 'pid', 'setpoint'),
      p: forwardData(hub, 'pid', 'p'),
      i: forwardData(hub, 'pid', 'i'),
      d: forwardData(hub, 'pid', 'd'),
      value: forwardData(hub, 'pid', 'value'),
      correction: () => new Observable(observer => {
        const listener = hub.on('pid/correction', (hub, data) => {
          observer.next(data)
        })
        return () => {
          listener.remove()
        }
      })
    }),
}

const remoteControlDive = socket => Command()
  .named('remote control dive')
  .require('dive')
  .makeCancelable()
  .action(system => fromEvent(socket, 'dive').pipe(
    map(amount => system.dive.power(amount))
  ))

const remoteControlTank = socket => Command()
  .named('remote control tank')
  .require('tank')
  .makeCancelable()
  .action(system => merge(
    fromEvent(socket, 'throttle').pipe(
      map(amount => system.tank.throttle(amount))
    ),
    fromEvent(socket, 'steering').pipe(
      map(amount => system.tank.steering(amount))
    ),
    fromEvent(socket, 'left').pipe(
      map(amount => system.tank.left(amount))
    ),
    fromEvent(socket, 'right').pipe(
      map(amount => system.tank.right(amount))
    )
  ))

const imuPIDDrive = socket => Command()
  .named('imu pid drive')
  .require('tank')
  .require('imu')
  .makeCancelable()
  .action(system =>  merge(
    fromEvent(socket, 'pid/setpoint').pipe(
      map(amount => system.pid.setpoint(amount))
    ),
    fromEvent(socket, 'pid/p').pipe(
      map(amount => system.pid.p(amount))
    ),
    fromEvent(socket, 'pid/i').pipe(
      map(amount => system.pid.i(amount))
    ),
    fromEvent(socket, 'pid/d').pipe(
      map(amount => system.pid.d(amount))
    ),
    system.imu.rotation().pipe(
      throttleTime(200),
      map(data => {
        let value = 0//TODO: convert data to value
        system.pid.value(value)
      })
    ),
    system.pid.correction.pipe(
      map(amount => system.tank.steering(amount)))
  ))
  
const listenToRotation = socket => Command()
  .named('imu listen')
  .require('imu')
  .require('pid')
  .makeCancelable()
  .action(system => {
    console.log('running imu')
    return system.imu.rotation().pipe(
      throttleTime(200),
      map(data => {
        //console.log('imu', data)
        socket.emit('imu/rotation', data)
      })  
    )
  })

init()

http.listen(1234, () => console.log('Web app listening on port 3000'))
