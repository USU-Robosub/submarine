const { Subsystem, Command } = require('../scheduler')
const { merge } = require('rxjs')
const { map, timeInterval } = require('rxjs/operators')
const pid = require('../PID/pid')

module.exports = (hub, handlerName="dive") => {

  let depthEnable = false
  let depthPidController = pid(1, 0, 0)
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
          depthEnable = !!amount
          depthTarget = amount?amount:0
        },
        pitch: amount => {
          pitchEnable = !!amount
          pitchTarget = amount?amount:0
        },
        setDepthPidGains: (p, i, d) => {
          depthPidController = pid(p, i, d)
        },
        setPitchPidGains: (p, i, d) => {
          pitchPidController = pid(p, i, d)
        }
      }),
    defaultCommand: scheduler => {
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
                  hub.emit(handlerName + '/power', depthPidController.correctFor(pressure - depthTarget, deltaTime / 1000.0))
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
      )
    }
  }
}
