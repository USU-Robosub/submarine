const { Command } = require('../scheduler')
const { map, debounceTime, tap } = require('rxjs/operators')
const { pipe, fromEvent, merge, zip } = require('rxjs')
const { parallel } = require('../scheduler/tools')

const dive = socket => Command()
  .named('remote control dive')
  .require('dive')
  .makeCancelable()
  .action(system => merge(
    fromEvent(socket, 'dive').pipe(
      map(amount => system.dive.power(amount ? amount : 0))
    ),
    fromEvent(socket, 'dive/steer').pipe(
      map(amount => system.dive.steering(amount ? amount : 0))
    ),
  ))

const tank = socket => Command()
  .named('remote control tank')
  .require('tank')
  .makeCancelable()
  .action(system => merge(
    fromEvent(socket, 'throttle').pipe(
      map(amount => system.tank.throttle(amount ? amount : 0))
    ),
    fromEvent(socket, 'steering').pipe(
      map(amount => system.tank.steering(amount ? amount : 0))
    ),
    fromEvent(socket, 'left').pipe(
      map(amount => system.tank.left(amount ? amount : 0))
    ),
    fromEvent(socket, 'right').pipe(
      map(amount => system.tank.right(amount ? amount : 0))
    ),
    fromEvent(socket, 'heading').pipe(
      map(angle => system.tank.heading(angle))
    )
  ))
  
const readPose = socket => Command()
  .named('remote control read pos')
  .require('pose')
  .makeCancelable()
  .action(system => {
    return merge(
      system.pose.yaw().pipe(
        tap(angle => socket.emit('pose/yaw', angle))  
      ),
      system.pose.pitch().pipe(
        tap(angle => socket.emit('pose/pitch', angle))  
      ),
      system.pose.roll().pipe(
        tap(angle => socket.emit('pose/roll', angle))  
      ),
      
      zip(
        system.pose.yaw(),
        system.pose.pitch(),
        system.pose.roll(),
      ).pipe(
        tap(angles => socket.emit('pose/all', angles))  
      ),
      
      system.pose.north().pipe(
        tap(angle => socket.emit('pose/north', angle))  
      ),
      system.pose.down().pipe(
        tap(angle => socket.emit('pose/down', angle))  
      ),
      system.pose.flatNorth().pipe(
        tap(angle => socket.emit('pose/flatNorth', angle))  
      ),
    )
  })

module.exports = {
  dive,
  tank,
  readPose
}
