const { scheduler:{ Command,  tools: { parallel, frc, mirror, concurrent, sequential } } } = require('./utils')
const { map, delay } = require('rxjs/operators')
const { timer, Subject, pipe, empty, of } = require('rxjs')

// const ai = Command()
//   .named('AI')
//   .action((system, _, log) =>
//     mirror({
//       worker: Command()
//         .named('AI maintain depth')
//         .require('dive')
//         .action(system => {
//           system.dive.depth(130)
//           return empty()
//         }), // TODO change to pid
//       main: Command()
//         .named('AI go through gate')
//         .require('tank')
//         .action(system => {
//           system.tank.heading(0.8) // TODO change to actual heading angle
//         })
//     })
//   )
  
const waitSeconds = time => Command()
  .named('AI wait for time')
  .action(system => {
    return timer(time * 1000)
  })
  
const ai = 
    sequential(
      Command()
        .named('AI set depth')
        .require('dive')
        .action(system => {
          system.dive.depth(130)
          console.log('Set depth')
          return empty()
        }),
      waitSeconds(1),
      Command()
        .named('AI turn to heading')
        .require('tank')
        .action(system => {
          system.tank.heading(0.8)
          console.log('Set turn')
          return empty()
        }),
      waitSeconds(5),
      Command()
        .named('AI go forward')
        .require('tank')
        .action(system => {
          system.tank.throttle(0.5)
          console.log('Set forward')
          return empty()
        }),
      waitSeconds(20),
      Command()
        .named('AI stop')
        .require('tank', 'dive')
        .action(system => {
          system.tank.throttle(0)
          system.tank.steering(0)
          system.dive.power(0)
          console.log('Set off')
          return empty()
        }),
  )
  
const waitCount = 2
  
const testAi = sequential(
  Command()
    .require('power')
    .action(system => {
      system.power.enable()
      return empty()
    }),
  waitSeconds(2),
  Command()
    .require('dive')
    .action(system => {
      system.dive.power(0.01)
      return empty()
    }),
  waitSeconds(waitCount),
  Command()
    .require('dive')
    .action(system => {
      system.dive.power(0)
      return empty()
    }),
  waitSeconds(waitCount),
  Command()
    .require('dive')
    .action(system => {
      system.dive.power(-0.01)
      return empty()
    }),
  waitSeconds(waitCount),
  Command()
    .require('dive')
    .action(system => {
      system.dive.power(0)
      return empty()
    }),
  waitSeconds(waitCount),
  Command()
    .require('tank')
    .action(system => {
      system.tank.throttle(0.01)
      return empty()
    }),
  waitSeconds(waitCount),
  Command()
    .require('tank')
    .action(system => {
      system.tank.throttle(0)
      return empty()
    }),
  waitSeconds(waitCount),
  Command()
    .require('tank')
    .action(system => {
      system.tank.throttle(-0.01)
      return empty()
    }),
  waitSeconds(waitCount),
  Command()
    .require('tank', 'power')
    .action(system => {
      system.tank.throttle(0)
      system.power.disable()
      return empty()
    }),
)

module.exports = {
  ai: testAi
}

/*
frc({
  init: () => {},
  execute: () => {},
  isFinished: () => {}
  end: () => {},
})
*/

/*
Command()
  .named('ai')
  .require('killSwitch','dive','tank')
  .makeCancelable()
  .action(system => {
    console.log('AI script ready')
    const killswitchStatus = system.killSwitch.status()
    const killSwitchOn = killswitchStatus.pipe(
      tap(status => console.log('killswitch', status)),
      filter(status => status == true)
    )
    const aiEnable = killSwitchOn.pipe(
      buffer(killSwitchOn.pipe(
        debounceTime(2000)
      )),
      map(list => list.length),
      filter(x => x >= 2),
      map(status => {
        console.log('Enabled AI')
        return true
      })
    )
    const aiDisable = killswitchStatus.pipe(
      filter(status => status == false),
      tap(() =>
        console.log('Disable AI'))
    )
    const aiControl = merge(aiEnable, aiDisable)
    // ============== AI SCRIPT ============

    let count = 0;
    const ai = interval(10).pipe(
      withLatestFrom(aiControl),
      map(([index, enabled]) => {
      if(enabled && count < 1000) {
        system.dive.power(-0.7)
        system.tank.throttle(0.5)
        system.tank.steering(0.3)
        system.dive.steering(0.1)
        count++;
      } else {
        if(!enabled)
          count = 0;
        system.dive.power(0)
        system.dive.steering(0)
        system.tank.throttle(0)
        system.tank.steering(0)
      }
      }))

    // ============== AI SCRIPT ============
    return ai
  })
*/
