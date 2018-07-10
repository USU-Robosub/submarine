const { Command } = require('./index')
const { tap, finalize, first, skip, takeUntil, filter, refCount, publish, map, concatMap, take } = require('rxjs/operators')
const { pipe, merge, concat, interval, range } = require('rxjs')

const parallel = (...pipes) => source =>
  merge(...pipes.map(pipe => source.pipe(pipe)))

const operatorConcat = (...operators) => source => concat(...operators.map(operator => operator(source)))

const self = func => source => func(source)(source)

const completeWhen = operators => pipe(
  publish(),
  refCount(),
  self(self =>
    takeUntil(self.pipe(operators))
  ),
)

const frc = (action, { base=interval(20) }={}) => system => base.pipe(
  parallel(
    pipe(
      first(),
      map(() => action.init ? action.init(system) : (action.execute ? action.execute(system) : null))
    ),
    pipe(
      skip(1),
      map(() => action.execute ? action.execute(system) : null),
    )
  ),
  completeWhen(
    pipe(
      filter(() => action.isFinished ? action.isFinished(system) : null)
    )
  ),
  finalize(() => action.end ? action.end(system) : null),
)

// const sequential = (...commands) => Command().action(
//   system => concat(...commands.map(command => command.internal.createObservable(system)))
// )

const count = () => {
  let counter = 0
  return map(() => counter++)
}

const sequential = (...commands) => {
  let scheduler = null
  let base = range(0, commands.length)
  const command = {
    ...Command().action(
      () => base.pipe(
        take(commands.length),
        count(),
        concatMap(index => {
          const instance = scheduler.run(commands[index])
          return instance.toObservable()
        })
      )
    ),
    bind: x => {
      scheduler = x
      return command
    },
    base: x => {
      base = x
      return command
    }
  }
  return command
}

const concurrent = (...commands) => Command().action(
  system => merge(...commands.map(command => command.internal.createObservable(system)))
)

module.exports = {
  frc,
  sequential,
  concurrent
}
