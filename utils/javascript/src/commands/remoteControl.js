const { Command } = require('../scheduler')
const { map } = require('rxjs/operators')
const { fromEvent, merge } = require('rxjs')

const dive = socket => Command()
  .named('remote control dive')
  .require('dive')
  .makeCancelable()
  .action(system => fromEvent(socket, 'dive').pipe(
    map(amount => system.dive.power(amount))
  ))

const tank = socket => Command()
  .named('remote control tank')
  .require('tank')
  .makeCancelable()
  .action(system => merge(
    fromEvent(socket, 'throttle').pipe(
      map(amount => system.tank.throttle(amount))
    ),
    fromEvent(socket, 'steering').pipe(
      map(amount => system.tank.steering(amount))
    ),
    fromEvent(socket, 'left').pipe(
      map(amount => system.tank.left(amount))
    ),
    fromEvent(socket, 'right').pipe(
      map(amount => system.tank.right(amount))
    )
  ))

module.exports = {
  dive,
  tank
}
