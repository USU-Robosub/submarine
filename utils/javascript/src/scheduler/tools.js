const { Command } = require('./index')
const { finalize, first, skip, map, concatMap, take, mergeMap, toArray, delay } = require('rxjs/operators')
const { pipe, merge, concat, interval, range, of, Observable, timer, from } = require('rxjs')


const sequential = (...commands) => {
  let base = range(0, commands.length)
  const command = {
    ...Command().action(
      (system, scheduler) => base.pipe(
        take(commands.length),
        count(),
        concatMap(index => {
          const instance = scheduler.run(commands[index])
          return instance.to.observable()
        })
      )
    ),
    base: x => {
      base = x
      return command
    }
  }
  return command
}

const concurrent = (...commands) => {
  let base = of(0)
  const command = {
    ...Command().action(
      (system, scheduler) => base.pipe(
        take(1),
        mergeMap(() => {
          return merge(...commands.map(command => {
            const instance = scheduler.run(command)
            if(instance.lockingFailed){
              throw Error(instance)
            }
            return instance.to.observable()
          }))
        })
      )
    ),
    base: x => {
      base = x
      return command
    }
  }
  return command
}

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
  takeWhileInclusive(() => !action.isFinished || !action.isFinished(system)),
  finalize(() => action.end ? action.end(system) : null),
  toArray(),
)

const once = action => system => of(0).pipe(map(() => action(system)))

const delayThen = (action, time=1) => system => timer(time).pipe(map(() => action(system)))

const repeatWithDelay = (action, time=1) => system => interval(time).pipe(map(() => action(system)))

const repeat = action => system => repeatWithDelay(action, 1)(system)

const fromPromise = action => system => from(action(system))

module.exports = {
  sequential,
  concurrent,
  frc,
  once,
  delayThen,
  repeat,
  repeatWithDelay,
  fromPromise,
}

const count = () => {
  let counter = 0
  return map(() => counter++)
}

const parallel = (...pipes) => source =>
  merge(...pipes.map(pipe => source.pipe(pipe)))

const takeWhileInclusive = predicate => source => new Observable(observer => {
  return source.subscribe({
    next(x) {
      if(predicate()){
        observer.next(x)
      }else{
        observer.next(x)
        observer.complete()
      }
    },
    error(err) { observer.error(err) },
    complete() { observer.complete() }
  })
})
