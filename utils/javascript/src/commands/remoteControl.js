const { Command } = require('../scheduler')
const { map, debounceTime, tap } = require('rxjs/operators')
const { pipe, fromEvent, merge, zip } = require('rxjs')
const { parallel } = require('../scheduler/tools')

const dive = socket => Command()
  .named('remote control dive')
  .require('dive')
  .makeCancelable()
  .action(system => merge(
    fromEvent(socket, 'dive/power').pipe(
      map(amount => system.dive.power(amount ? amount : 0))
    ),
    fromEvent(socket, 'dive/trim').pipe(
      map(amount => system.dive.trim(amount ? amount : 0))
    ),
    fromEvent(socket, 'dive/depth').pipe(
      map(amount => system.dive.depth(amount))
    ),
    fromEvent(socket, 'dive/pitch').pipe(
      map(angle => system.dive.pitch(angle))
    ),
    fromEvent(socket, 'dive/pitch/pid').pipe(
      map(p, i, d => system.dive.setPitchPidGains(p, i, d))
    ),
    fromEvent(socket, 'dive/depth/pid').pipe(
      map(p, i, d => system.dive.setDepthPidGains(p, i, d))
    )
  ))

const tank = socket => Command()
  .named('remote control tank')
  .require('tank')
  .makeCancelable()
  .action(system => merge(
    fromEvent(socket, 'tank/throttle').pipe(
      map(amount => system.tank.throttle(amount ? amount : 0))
    ),
    fromEvent(socket, 'tank/steering').pipe(
      map(amount => system.tank.steering(amount ? amount : 0))
    ),
    fromEvent(socket, 'tank/left').pipe(
      map(amount => system.tank.left(amount ? amount : 0))
    ),
    fromEvent(socket, 'tank/right').pipe(
      map(amount => system.tank.right(amount ? amount : 0))
    ),
    fromEvent(socket, 'tank/heading').pipe(
      map(angle => system.tank.heading(angle))
    ),
    fromEvent(socket, 'tank/heading/pid/angle').pipe(
      map(p, i, d => system.tank.setHeadingPidGains(p, i, d))
    ),
    fromEvent(socket, 'tank/heading/pid/velocity').pipe(
      map(p, i, d => system.tank.setHeadingVelocityPidGains(p, i, d))
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
      system.pose.yawVelocity().pipe(
        tap(angle => socket.emit('pose/yawVelocity', angle))
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
