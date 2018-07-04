const { interval, Subject } = require('rxjs')
const { publish, takeUntil } = require('rxjs/operators')

const Scheduler = (subsystems) => {
  const locks = []

  return {
    run: command => {
      // type checks
      if(typeof command == 'undefined' || command == null)
        throw Error('Must pass a command')

      if(typeof command.action != 'function')
        throw Error('Command must have an action to perform')

      if(!Array.isArray(command.requires))
        throw Error('Command must require an array of subsystems')

      if(typeof command.name != 'string')
        throw Error('Command must have a name')

      // lock checks
      const lockConflicts = command.requires // TODO check if commands locking subsystem can be canceled
        .map(name => [ name, subsystems[name] ])
        .filter(([ name, subsystem ]) => subsystem.mustBeLocked)
        .filter(([ name, subsystem ]) => locks.findIndex(lock => lock.name == name) >= 0)
        .map(([ name, subsystem ]) => [name, subsystem, locks.find(lock => lock.name == name).command])

      if(lockConflicts.length > 0){
        const [ subsystemName, lockedSubsystem, lockingCommand ] = lockConflicts[0]
        throw Error('Command \"' + command.name + '\" attempted to lock subsystem \"' + subsystemName + '\" when it is already locked by command \"' + lockingCommand.name + '\"')
      }

      // lock subsystems
      // TODO cancel locks for subsystems

      command.requires.forEach(name => locks.push({ name, command })) // TODO does not work for sharable subsystems

      // create running command
      const required = command.requires.reduce((obj, name) => ({ ...obj, [name]: subsystems[name] }), {})

      const cancelSubject = new Subject();
      const observable = interval().pipe(
        command.action(required),
        takeUntil(cancelSubject),
        publish()
      )
      observable.connect()
      return {
        cancel: () => {
          cancelSubject.next()
        }
      }
    }
  }
}

module.exports = Scheduler
