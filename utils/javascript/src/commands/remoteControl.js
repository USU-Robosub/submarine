const { Command } = require('../scheduler')
const { map, debounceTime } = require('rxjs/operators')
const { pipe, fromEvent, merge } = require('rxjs')
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
    fromEvent(socket, 'steering').pipe(
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
    )
  ))

module.exports = {
  dive,
  tank
}
