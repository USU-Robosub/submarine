const { Subsystem, Command } = require('../scheduler')
const { map, timeInterval, tap, withLatestFrom } = require('rxjs/operators')
const pid = require('../PID/pid')

function absMod(x, n){
  return x - Math.floor(x/n) * n
}

function angleBetween(a, b){
  return absMod((a - b) + Math.PI, Math.PI * 2) - Math.PI
}

module.exports = (hub, handlerName="tank") => {

  let headingCorrectionEnabled = false
  let headingPidController = pid(0.5,0.06,0.04)
  let headingTarget = 0

  let headingVelocityPidController = pid(0.12, 0, 0)

  return { subsystem: Subsystem()
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
      setHeadinVelocitygPidGains: (p, i, d) => {
        headingVelocityPidController = pid(p, i, d)
      },
      heading: angle => {
        headingCorrectionEnabled = angle!==false
        headingTarget = angle!==false?angle:0
      },
    }),
    defaultCommand: (scheduler => {
      scheduler.build(Command()
        .named('tank heading correction')
        .makeCancelable()
        .require('pose')
        .action((system, scheduler, log) => {
          return system.pose.yaw().pipe(
            withLatestFrom(system.pose.yawVelocity()),
            timeInterval(),
            map(({ value:[ angle, angleVelocity ], interval:deltaTime }) => {
              if(headingCorrectionEnabled){
                let headingCorrection = headingPidController.correctFor(angleBetween(angle, headingTarget), deltaTime / 1000.0)

                let headingVelocityCorrection = -headingVelocityPidController.correctFor(angleBetween(headingCorrection, angleVelocity), deltaTime / 1000.0)
                hub.emit(handlerName + '/steering',[headingVelocityCorrection])
              }
            })
          )
        })).then().run().to.promise().catch(e => console.error(e))
    })
  }
}
