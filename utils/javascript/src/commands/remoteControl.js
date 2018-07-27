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
      // parallel(
      //   amount => amount,
      //   pipe(
      //     debounceTime(2000),
      //     map(amount => 0)
      //   )
      // ),
      map(amount => system.dive.power(amount))
    ),
    fromEvent(socket, 'dive/steer').pipe(
    // parallel(
    //   amount => amount,
    //   pipe(
    //     debounceTime(2000),
    //     map(amount => 0)
    //   )
    // ),
      map(amount => system.dive.steering(amount))
    ),
  ))

const tank = socket => Command()
  .named('remote control tank')
  .require('tank')
  .makeCancelable()
  .action(system => merge(
    fromEvent(socket, 'throttle').pipe(
    // parallel(
    //   amount => amount,
    //   pipe(
    //     debounceTime(2000),
    //     map(amount => 0)
    //   )
    // ),
      map(amount => system.tank.throttle(amount))
    ),
    fromEvent(socket, 'steering').pipe(
    // parallel(
    //   amount => amount,
    //   pipe(
    //     debounceTime(2000),
    //     map(amount => 0)
    //   )
    // ),
      map(amount => system.tank.steering(amount))
    ),
    fromEvent(socket, 'left').pipe(
    // parallel(
    //   amount => amount,
    //   pipe(
    //     debounceTime(2000),
    //     map(amount => 0)
    //   )
    // ),
      map(amount => system.tank.left(amount))
    ),
    fromEvent(socket, 'right').pipe(
    // parallel(
    //   amount => amount,
    //   pipe(
    //     debounceTime(2000),
    //     map(amount => 0)
    //   )
    // ),
      map(amount => system.tank.right(amount))
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
        tap(angle => socket.emit('pose/calibratedYaw', angle))  
      ),
      system.pose.calibratedYaw().pipe(
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
      
      zip(
        system.pose.calibratedYaw(),
        system.pose.pitch(),
        system.pose.roll(),
      ).pipe(
        tap(angles => socket.emit('pose/calibratedAll', angles))  
      ),
      
      system.pose.north().pipe(
        tap(angle => socket.emit('pose/north', angle))  
      ),
      system.pose.calibratedNorth().pipe(
        tap(angle => socket.emit('pose/calibratednorth', angle))  
      ),
      system.pose.down().pipe(
        tap(angle => socket.emit('pose/down', angle))  
      ),
      system.pose.flatNorth().pipe(
        tap(angle => socket.emit('pose/flatNorth', angle))  
      ),
      system.pose.flatForward().pipe(
        tap(angle => socket.emit('pose/flatForward', angle))  
      ),
      system.pose.flatCalibratedNorth().pipe(
        tap(angle => socket.emit('pose/flatCalibratedNorth', angle))  
      )
    )
  })

module.exports = {
  dive,
  tank,
  readPose
}
