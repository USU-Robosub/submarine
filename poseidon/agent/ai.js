const { scheduler:{ Command,  tools: { parallel, frc, mirror, concurrent, sequential } } } = require('./utils')
const { map } = require('rxjs/operators')
const { Subject, pipe } = require('rxjs')

const ai = Command()
  .named('AI')
  .action((system, _, log) =>
    mirror({
      worker: Command()
        .named('AI maintain depth')
        .require('dive')
        .action(system => system.dive.power(-0.5)), // TODO change to pid
      main: Command()
        .named('AI go through gate')
        .require('tank')
        .action(system => {
          system.tank.throttle(0.5)
          system.tank.heading(0) // TODO change to actual heading angle
        })
    })
  )

module.exports = {
  ai
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
