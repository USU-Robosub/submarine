const { Subsystem, Command } = require('../scheduler')
const { merge } = require('rxjs')
const { map, timeInterval } = require('rxjs/operators')
const pid = require('../PID/pid')

module.exports = (hub, handlerName="dive") => {

  let depthEnable = false
  let depthPidController = pid(0.002, 0.001, 0.0011)
  let depthTarget = 0

  let pitchEnable = false
  let pitchPidController = pid(1, 0, 0)
  let pitchTarget = 0

  return {
    subsystem: Subsystem()
      .named('dive')
      .raw({
        power: amount => {
          depthEnable = false
          hub.emit(handlerName + '/power', [amount])
        },
        trim: amount => {
          pitchEnable = false
          hub.emit(handlerName + '/steering', [amount])
        },
        depth: amount => {
          console.log("dive/depth", amount)
          depthEnable = amount!==false
          depthTarget = amount?amount:0
          console.log("dive/depth", depthEnable, depthTarget)
        },
        pitch: amount => {
          pitchEnable = amount!==false
          pitchTarget = amount?amount:0
        },
        setDepthPidGains: ([p, i, d]) => {
          depthPidController = pid(p, i, d)
          console.log("depth/pidGains", p, i, d)
        },
        setPitchPidGains: ([p, i, d]) => {
          pitchPidController = pid(p, i, d)
        }
      }),
    defaultCommand: scheduler => {
      console.log("starting dive pid")
      scheduler.build(Command()
        .named('dive depth correction')
        .makeCancelable()
        .require('pose', 'imu')
        .action(system => {
          return merge(
            system.imu.pressure().pipe(
              timeInterval(),
              map(({ value: pressure, interval: deltaTime }) => {
                if(depthEnable){
                  
                  let power = depthPidController.correctFor(pressure - depthTarget, deltaTime / 1000.0)
                  let offsetPower = power - 0.1172413 // TODO this is for the submarine right now (makes it always push down to hold depth)
                  //console.log("dive/pid/power", power)
                  hub.emit(handlerName + '/power', offsetPower)
                }
              })
            ),
            system.pose.pitch().pipe(
              timeInterval(),
              map(({ value: pitch, interval: deltaTime }) => {
                if(pitchEnable){
                  hub.emit(handlerName + '/trim', pitchPidController.correctFor(pitch - pitchTarget, deltaTime / 1000.0))
                }
              })
            )
          )
        })
      ).then().run().to.promise().catch((e) => {
        console.log(e)
      })
    }
  }
}
