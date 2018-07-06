const { interval, Subject, from } = require('rxjs')
const { publish, takeUntil, catchError, finalize } = require('rxjs/operators')

const trace = command => command.innerCommands ? ({ [command.name]: command.innerCommands().map(command => trace(command)) }) : command.name

const Scheduler = (subsystems) => {
  let instances = []
  return {
    run: (command, config) => {
      checkForValidCommand(command)
      removeCancelableLocks(instances, subsystems, command)
      checkForLockConflicts(instances, subsystems, command)
      const instance = createCommandInstance(instances, subsystems, command, config, () => instances.splice(instances.indexOf(instance), 1))
      instances.push(instance)
      return {
        toPromise: () => instance.internal.observable.toPromise(),
        toObservable: () => instance.internal.observable,
        cancelable: instance.cancelable,
        step: instance.step,
        cancel: instance.cancel,
        abort: instance.abort
      }
    },
    running: () => {
      return instances.map(instance => trace(instance.internal.command))
    }
  }
}

module.exports = Scheduler

const propWithDefault = (obj, prop, def) => typeof obj != 'undefined' && typeof obj[prop] != 'undefined' ? obj[prop] : def
const propIsTrue = (obj, prop) => typeof obj != 'undefined' && typeof obj[prop] != 'undefined' && obj[prop] === true

const generateBaseObservable = (config, action) => {
  if(actionNotPipeOperator(action)){
    return {
      base: from(action)
    }
  }else if(propIsTrue(config, 'manual')){
    const manualSubject = new Subject()
    return {
      base: manualSubject.pipe(action),
      step: () => manualSubject.next()
    }
  }else{
    return {
      base: interval(propWithDefault(config, 'interval', 1000 / 60)).pipe(action)
    }
  }
}
const actionNotPipeOperator = action => typeof action != 'function'

const createCommandInstance = (instances, subsystems, command, config, removeInstance) => {
  const required = command.requires.reduce((obj, name) => ({ ...obj, [name]: subsystems[name] }), {})
  const cancelSubject = new Subject();
  const action = command.action(required)
  const { base:baseObservable, step:observableStep } = generateBaseObservable(config, action)
  const cancelable = actionNotPipeOperator(action) ? false : command.cancelable
  const observable = baseObservable.pipe(
    takeUntil(cancelSubject),
    finalize(() => removeInstance()),
    publish(),
  )
  observable.connect()
  const instance = {
    internal:{
      command,
      observable,
      locks: command.requires.filter(name => subsystems[name].mustBeLocked),
    },
    cancelable,
    cancel: () => {
      if(!cancelable)
        throw Error('Can only cancel a command marked as cancelable. The command \"' + command.name + '\" is not marked cancelable')

      cancelSubject.next()
    },
    abort: () => {
      cancelSubject.next()
    },
    step: () => {
      if(notDefined(observableStep))
        throw Error('Can only step an instance created with manual flag')

      observableStep()
    }
  }
  return instance
}
const notDefined = x => typeof x == 'undefined'

const removeCancelableLocks = (instances, subsystems, command) => {
  const cancelableInstances = command.requires
    .filter(subsystemName => subsystems[subsystemName].mustBeLocked)
    .filter(subsystemName => instances.some(
      instance => instance.internal.locks.some(name => name == subsystemName)
        && instance.internal.command.cancelable)
    )
    .map(subsystemName => [
      subsystemName,
      subsystems[subsystemName],
      instances.find(
        instance => instance.internal.locks.some(name => name == subsystemName)
      )
    ])

  cancelableInstances.forEach(([ name, subsystem, instance ]) => instance.cancel())
}

const checkForLockConflicts = (instances, subsystems, command) => {
  const lockConflicts = command.requires
    .filter(subsystemName => subsystems[subsystemName].mustBeLocked)
    .filter(subsystemName => instances.some(
      instance => instance.internal.locks.some(name => name == subsystemName))
    )
    .map(subsystemName => [
      subsystemName,
      subsystems[subsystemName],
      instances.find(
        instance => instance.internal.locks.some(name => name == subsystemName)
      )
    ])

  if(lockConflicts.length > 0){
    const [ subsystemName, lockedSubsystem, lockingInstance ] = lockConflicts[0]
    throw Error('Command \"' + command.name + '\" attempted to lock subsystem \"' + subsystemName + '\" when it is already locked by command \"' + lockingInstance.internal.command.name + '\"')
  }
}

const checkForValidCommand = command => {
  if(typeof command == 'undefined' || command == null)
    throw Error('Must pass a command')

  if(typeof command.action != 'function')
    throw Error('Command must have an action to perform')

  if(!Array.isArray(command.requires))
    throw Error('Command must require an array of subsystems')

  if(typeof command.name != 'string')
    throw Error('Command must have a name')
}
