const { tap, finalize, first, skip, takeUntil, filter, refCount, publish, map } = require('rxjs/operators')
const { pipe, merge, concat } = require('rxjs')

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

const convertFRC_LikeCommand = (name, command) => ({
  name,
  requires: command.requires,
  cancelable: command.cancelable,
  action: requires => pipe(
    parallel(
      pipe(
        first(),
        map(() => command.init ? command.init(requires) : (command.execute ? command.execute(requires) : null))
      ),
      pipe(
        skip(1),
        map(() => command.execute ? command.execute(requires) : null),
      )
    ),
    completeWhen(
      pipe(
        filter(() => command.isFinished ? command.isFinished(requires) : null)
      )
    ),
    finalize(() => command.end ? command.end(requires) : null),
  )
})

const sequential = (name, ...commands) => {
  let currentCommand = null
  return {
    name: name,
    requires: commands.reduce((array, command) => array.concat(command.requires), []),
    innerCommands: () => {
      return currentCommand ? [currentCommand] : []
    },
    action: requires => pipe(
      operatorConcat(...commands.map(command => pipe(
        tap(() => { currentCommand = command }),
        command.action(requires),
      ))),
    )
  }
}

const concurrent = (name, ...commands) => {
  const currentCommands = []
  return {
    name: name,
    requires: commands.reduce((array, command) => array.concat(command.requires), []),
    innerCommands: () => {
      return currentCommands
    },
    action: requires => pipe(
      parallel(...commands.map(command => pipe(
        tap(() => { if(currentCommands.indexOf(command) == -1) currentCommands.push(command) }),
        command.action(requires),
        finalize(() => { currentCommands.splice(currentCommands.indexOf(command), 1) }),
      ))),
    )
  }
}

module.exports = {
  convertFRC_LikeCommand,
  sequential,
  concurrent
}
