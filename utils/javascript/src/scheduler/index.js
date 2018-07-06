const { interval, Subject, from, Observable, of } = require('rxjs')
const { publish, takeUntil, catchError, finalize } = require('rxjs/operators')

function Scheduler(subsystems){
  let instances = []
  const scheduler = {
    run: (command, config) => {
      checkForValidCommand(command, subsystems)
      removeCancelableLocks(instances, subsystems, command)
      checkForLockConflicts(instances, subsystems, command)
      const instance = createCommandInstance(instances, subsystems, command, config,
        () => {
          instances.splice(instances.indexOf(instance), 1)
          instance.internal.locks.forEach(name => {
            if(defined(subsystems[name].commands) && defined(subsystems[name].commands.default)){
              if(subsystems[name].commands.default != instance.internal.command){ // don't start default if just stopped it
                scheduler.run(subsystems[name].commands.default)
              }
            }
          })
        }
      )
      instances.push(instance)
      if(!instance.public.stepable){
        instance.internal.start()
      }
      return instance.public
    },
    running: () => {
      return instances.map(instance => trace(instance.internal.command))
    },
    abortAll: () => {
      instances.forEach(instance => instance.public.abort())
    }
  }
  startDefaultCommandsForSubsystems(subsystems, scheduler)
  return scheduler
}

const Command = (name, requires, action, {cancelable}={}) => ({name, requires, action, cancelable})

module.exports = {
  Scheduler,
  Command,
}

function startDefaultCommandsForSubsystems(subsystems = {}, scheduler){
  Object.entries(subsystems).forEach(([ name, subsystem ]) => {
    if(defined(subsystem.commands) && defined(subsystem.commands.default)){
      if(!propIsTrue(subsystem.commands.default, 'cancelable'))
        throw Error('Default command for subsystem \"' + name + '\" must be cancelable')

      scheduler.run(subsystem.commands.default)
    }
  })
}

function createActionObservable(config, command, requires, onFinished){
  const action = command.action(requires)
  const { base, step, stepable } = generateBaseObservable(config, action)
  const observable = base.pipe(
    finalize(() => onFinished()),
    publish(),
  )
  let subscription = null
  return {
    observable,
    start: () => subscription = observable.connect(),
    cancelable: command.cancelable,
    stop: () => subscription.unsubscribe(),
    stepable,
    step: () => {
      if(subscription === null){
        subscription = observable.connect()
      }
      if(defined(step)){
        step()
      }
    }
  }
}
const actionIsPipeOperator = action => typeof action == 'function'

function createCommandInstance(instances, subsystems, command, config, removeInstance){
  const requires = command.requires.reduce((obj, name) => ({ ...obj, [name]: subsystems[name] }), {})
  const { observable, start, cancelable, stop, stepable, step } = createActionObservable(config, command, requires, removeInstance)
  const instance = {
    internal:{
      start,
      command,
      observable,
      locks: command.requires.filter(name => subsystems[name].mustBeLocked),
    },
    public:{
      cancelable,
      cancel: () => {
        if(!cancelable) throw Error('Can only cancel a command marked as cancelable. The command \"' + command.name + '\" is not marked cancelable')
        stop()
      },
      abort: () => {
        stop()
      },
      stepable,
      step: () => {
        if(!stepable) throw Error('Can only step an instance created with manual flag')
        step()
      },
      toPromise: () => observable.toPromise(),
      toObservable: () => observable,
    }
  }
  return instance
}
const notDefined = x => typeof x == 'undefined'
const defined = x => typeof x != 'undefined'

const trace = command => command.innerCommands ? ({ [command.name]: command.innerCommands().map(command => trace(command)) }) : command.name

const propWithDefault = (obj, prop, def) => typeof obj != 'undefined' && typeof obj[prop] != 'undefined' ? obj[prop] : def
const propIsTrue = (obj, prop) => typeof obj != 'undefined' && typeof obj[prop] != 'undefined' && obj[prop] === true

function generateBaseObservable(config, action){
  if(action instanceof Observable){
    return {
      base: action,
      stepable: propIsTrue(config, 'manual'),
    }
  }else if(actionNotPipeOperator(action)){
    let base = null
    try{
      base = from(action)
    }catch(e){
      base = of(action)
    }
    return {
      base,
      stepable: false,
    }
  }else if(propIsTrue(config, 'manual')){
    const manualSubject = new Subject()
    return {
      base: manualSubject.pipe(action),
      step: () => manualSubject.next(),
      stepable: true,
    }
  }else{
    return {
      base: interval(propWithDefault(config, 'interval', 1000 / 60)).pipe(action),
      stepable: false,
    }
  }
}
const actionNotPipeOperator = action => typeof action != 'function'

function removeCancelableLocks(instances, subsystems, command){
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

  cancelableInstances.forEach(([ name, subsystem, instance ]) => instance.public.cancel())
}

function checkForLockConflicts(instances, subsystems, command){
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

const checkForValidCommand = (command, subsystems = []) => {
  if(typeof command == 'undefined' || command == null)
    throw Error('Must pass a command')

  if(typeof command.action != 'function')
    throw Error('Command must have an action to perform')

  if(!Array.isArray(command.requires))
    throw Error('Command must require an array of subsystems')

  if(typeof command.name != 'string')
    throw Error('Command must have a name')

  const subsystemNames = Object.keys(subsystems)
  command.requires.forEach(name => {
    if(!subsystemNames.includes(name)){
      throw Error('Command tried to require a subsystem \"' + name + '\" that does not exist')
    }
  })
}
