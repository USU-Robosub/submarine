const { publish } = require('rxjs/operators')

function Subsystem(raw={}){
  let shareable = false
  let defaultCommand = null
  let name = '<anonymous>'
  const subsystem = {
    internal:{
      isSubsystem: true,
      raw,
      shareable: () => shareable,
      hasDefaultCommand: () => defaultCommand !== null,
      defaultCommand: () => defaultCommand,
      name: () => name,
    },
    makeShared: () => {
      shareable = true
      return subsystem
    },
    whenIdleRun: command => {
      defaultCommand = command
      return subsystem
    },
    named: string => {
      name = string
      return subsystem
    },
  }
  return subsystem
}

function Command(){
  let requiredSubsystems = []
  let cancelable = false
  let createObservable = () => empty()
  let name = '<anonymous>'
  const command = {
    internal: {
      isCommand: true,
      requiredSubsystems: () => requiredSubsystems,
      cancelable: () => cancelable,
      createObservable: system => createObservable(system),
      name: () => name,
    },
    require: (...args) => {
      requiredSubsystems = mergeIntoOneArray(args)
      return command
    },
    makeCancelable: () => {
      cancelable = true
      return command
    },
    action: func => {
      createObservable = func
      return command
    },
    named: string => {
      name = string
      return command
    }
  }
  return command
}

function Scheduler(subsystems=[], {  }={}){
  subsystems.forEach(assertSubsystem)
  const instances = []
  const scheduler = {
    run: command => {
      assertCommand(command)
      removeCancelableLocks(command, instances)
      const result = checkForLockConflicts(command, instances)
      if(result.lockingFailed) return result
      const system = createSystemForCommand(command, subsystems)
      const { control, observable } = createControlForObservable(
        command.internal.createObservable(system),
        {onStart: () => {}, onStop: () => {
          instances.splice(instances.indexOf(instance), 1)
          restartDefaultCommands(instance.locks, scheduler, subsystems, command)
        }}
      )
      allowObservableToStopCommand(observable, control)
      const instance = createCommandInstance(command, control, observable, subsystems)
      instances.push(instance)
      control.start()
      return instance.public
    },
    runningByName: () => {
      return instances.map(instance => instance.command.internal.name())
    },
    running: () => {
      return instances
    }
  }
  startDefaultCommands(subsystems, scheduler)
  return scheduler
}

module.exports = {
  Subsystem,
  Command,
  Scheduler,
}

const getChildren = command => command.internal.getInstance

const startDefaultCommands = (subsystems, scheduler) => {
  subsystems.forEach(subsystem => {
    if(subsystem.internal.hasDefaultCommand()){
      scheduler.run(subsystem.internal.defaultCommand())
    }
  })
}

const restartDefaultCommands = (names, scheduler, subsystems, commandThatStopped) => {
  names.forEach(name => {
    const subsystem = findSubsystemByName(subsystems, name)
    if(subsystem.internal.hasDefaultCommand()
      && subsystem.internal.defaultCommand() !== commandThatStopped){
      scheduler.run(subsystem.internal.defaultCommand())
    }
  })
}

const findSubsystemByName = (subsystems, name) =>
  subsystems.find(subsystem => subsystem.internal.name() == name)

const removeCancelableLocks = (command, instances) => {
  const cancelableConflictingInstances = instances.filter(
    instance => instance.command.internal.cancelable()
      && instance.locks.some(
        name => command.internal.requiredSubsystems().includes(name)
      )
  )
  cancelableConflictingInstances.forEach(instance => instance.control.stop())
}

const createCommandInstance = (command, control, observable, subsystems) => {
  const locks = command.internal.requiredSubsystems().filter(
    name => !subsystems.find(subsystem => subsystem.internal.name() == name).internal.shareable()
  )
  const instance = {
    control,
    locks,
    command,
    public: {
      lockingFailed: false,
      cancelable: command.internal.cancelable(),
      cancel: () => {
        if(command.internal.cancelable()){
          control.stop()
          return true
        }
        return false
      },
      abort: () => {
        control.stop()
      },
      toPromise: () => observable.toPromise(),
      toObservable: () => observable,
    }
  }
  return instance
}

const checkForLockConflicts = (command, instances) => {
  const lockConflicts = instances.reduce(
    (conflicts, instance) => conflicts.concat(instance.locks.filter(
      name => command.internal.requiredSubsystems().includes(name)
    ).map(
      name => ({instance, subsystem: name})
    ))
  , [])
  if(lockConflicts.length > 0){
    return {
      lockingFailed: true,
      lockConflicts: lockConflicts.map(
        conflict => ({
          command: conflict.instance.command.internal.name(),
          subsystem: conflict.subsystem
        })
      )
    }
  }
  return {
    lockingFailed: false,
  }
}

const createSystemForCommand = (command, subsystems) => {
  const system = command.internal.requiredSubsystems().reduce((system, name) => {
    system[name] = subsystems.find(subsystem => subsystem.internal.name() == name).internal.raw
    return system
  }, {})
  return system
}

const createControlForObservable = (observable, { onStart, onStop }) => {
  const publishedObservable = observable.pipe(publish())
  const control = {
    subscription: null,
    started: false,
    stopped: false,
    stoppedWhenStarting: false,
    start: () => {
      if(!control.started){
        control.started = true
        onStart()
        control.subscription = publishedObservable.connect()
        if(control.stoppedWhenStarting){ // happens if the observable completes when connect() is called on it
          control.stop()
        }
      }
    },
    stop: () => {
      if(control.subscription == null){
        control.stoppedWhenStarting = true
      }else if(!control.stopped){
        control.stopped = true
        control.subscription.unsubscribe()
        onStop()
      }
    },
  }
  return {control, observable: publishedObservable}
}

const allowObservableToStopCommand = (observable, control) => {
  observable.subscribe(
    x => {},
    e => {
      control.stop()
    },
    () => {
      control.stop()
    },
  )
}

const assertSubsystem = subsystem => {
  if(!(subsystem.internal && subsystem.internal.isSubsystem))
    throw TypeError('Given subsystem that was not created with Subsystem()')
}

const assertCommand = command => {
  if(!(command.internal && command.internal.isCommand))
    throw TypeError('Given command that was not created with Command()')
}

const mergeIntoOneArray = arrayOfThings => arrayOfThings.reduce((array, thing) => {
  if(Array.isArray(thing)){
    return array.concat(thing)
  }else{
    array.push(thing)
    return array
  }
}, [])
