const { Subsystem, Command } = require('../scheduler')
const { map, timeInterval, tap } = require('rxjs/operators')
const pid = require('../PID/pid')

function absMod(x, n){
  return x - Math.floor(x/n) * n
}

function angleBetween(a, b){
  return absMod((a - b) + Math.PI, Math.PI * 2) - Math.PI
}

module.exports = (hub, handlerName="tank") => {
  
  let headingCorrectionEnabled = false
  const P = 5;
  const T = 4/0.2;
  let headingPidController = pid(0.6*P, 2*P/T, P*T/8)
  let headingTarget = 0
  
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
      heading: angle => {
        console.log('set heading', angle)
        headingCorrectionEnabled = true
        headingTarget = angle
      }
    }),
    defaultCommand: (scheduler => {
      scheduler.build(Command()
        .named('tank heading correction')
        .makeCancelable()
        .require('pose')
        .action(system => {
          return system.pose.yaw().pipe(
            timeInterval(),
            map(({ value:angle, interval:deltaTime }) => {
              if(headingCorrectionEnabled){
                let correction = headingPidController.correctFor(angleBetween(angle, headingTarget), deltaTime / 1000.0) * (0.5 / Math.PI)
                // console.log("Tank Pose Data", angle, headingTarget, angleBetween(angle, headingTarget), deltaTime,  correction)
                hub.emit(handlerName + '/steering', correction)
                // console.log('correction', correction, angle, headingTarget, angleBetween(angle, headingTarget))
              }
            })
          )
        })).then().run().to.promise().catch(e => console.error(e))
    })
  }
}