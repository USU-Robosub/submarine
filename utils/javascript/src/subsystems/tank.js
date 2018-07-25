const { Subsystem, Command } = require('../scheduler')
const { map, timeInterval } = require('rxjs/operators')
const pid = require('../PID/pid')

function absMod(x, n){
  return x - Math.floor(x/n) * n
}

function angleBetween(a, b){
  return absMod((a - b) + 180, 360) - 180
}

module.exports = (hub, handlerName="tank") => {
  
  let headingCorrectionEnabled = false
  let headingPidController = pid(1, 0, 0)
  let headingTarget = 0
  
  return Subsystem()
    .named('tank')
    .raw({
      throttle: amount => {
        hub.emit(handlerName + '/throttle', [amount])
      },
      steering: amount => {
        headingCorrectionEnabled = false
        hub.emit(handlerName + '/steering', [amount])
      },
      left: amount => {
        headingCorrectionEnabled = false
        hub.emit(handlerName + '/left', [amount])
      },
      right: amount => {
        headingCorrectionEnabled = false
        hub.emit(handlerName + '/right', [amount])
      },
      setHeadingPidGains: (p, i, d) => {
        headingPidController = pid(p, i, d)
      },
      heading: angle => {
        headingCorrectionEnabled = true
        headingTarget = angle
      }
    })
    .whenIdleRun(
      Command()
        .named('tank heading correction')
        .makeCancelable()
        .require('pose')
        .action(system => {
          return system.pose.yaw().pipe(
            timeInterval(),
            map(({ value:angle, interval:deltaTime }) => {
              hub.emit(handlerName + '/steering', pid.correctFor(angleBetween(angle, headingTarget), deltaTime / 1000.0) * (0.5 / 180))
            })
          )
        })
    )
}