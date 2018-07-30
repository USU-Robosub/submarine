const { publish, takeUntil } = require('rxjs/operators')
const { _throw, Subject } = require('rxjs')

function Subsystem(raw={}){
  let shareable = false
  let defaultCommand = null
  let name = '<anonymous>'
  let rawObj = raw
  const subsystem = {
    internal:{
      isSubsystem: true,
      raw: () => rawObj,
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
    raw: obj => {
      rawObj = obj
      return subsystem
    }
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
      createObservable: (system, scheduler) => createObservable(system, scheduler),
      name: () => name,
    },
    require: (...args) => {
      requiredSubsystems = requiredSubsystems.concat(mergeIntoOneArray(args))
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

const basicLog = {
  good: (...args) => console.log('[Good]', ...args),
  info: (...args) => console.log('[Info]', ...args),
  warn: (...args) => console.error('[Warn]', ...args),
  error: (...args) => console.error('[Error]', ...args)
}

function Scheduler(subsystems=[], { log }={ log:basicLog }){
  subsystems.forEach(assertSubsystem)
  const commands = []
  const instances = []
  const scheduler = {
    remember: (...args) => {
      const commandsToRemember = mergeIntoOneArray(args)
      commandsToRemember.forEach(command => {
        if(commands.findIndex(com => com.internal.name() == command.internal.name()) >= 0)
          throw Error('Already remembered a command with the name ' + command.internal.name())
        commands.push(command)
      })
    },
    run: command => {
      const instance = scheduler.build(command)
      if(instance.errors){
        return instance
      }
      return instance.run()
    },
    build: command => {
      if(Object.prototype.toString.call(command) === "[object String]"){
        if(commands.findIndex(com => com.internal.name() == command) < 0)
          throw Error('Haven\'t remembered a command with the name ' + command)
        command = commands.find(com => com.internal.name() == command)
      }
      assertCommand(command)
      let result = checkForMissingRequires(command, subsystems)
      if(result.errors) return result
      removeCancelableLocks(command, instances)
      result = checkForLockConflicts(command, instances)
      if(result.errors) return result
      const system = createSystemForCommand(command, subsystems)
      const { control, observable } = createControlForObservable(
        command.internal.createObservable(system, scheduler, log),
        {onStart: () => {}, onStop: () => {
          instances.splice(instances.indexOf(instance), 1)
          restartDefaultCommands(instance.locks, scheduler, subsystems, command)
        }}
      )
      allowObservableToStopCommand(observable, control)
      const instance = createCommandInstance(command, control, observable, subsystems)
      instances.push(instance)
      const withRun = {
        ...instance.public,
        run: () => {
          instance.control.start()
          return withRun
        },
        then: handler => {
          instance.public.then(handler)
          return withRun
        }
      }
      return withRun
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
  let promise = null
  const instance = {
    control,
    locks,
    command,
    public: {
      errors: null,
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
      then: handler => {
        if(promise === null){
          promise = observable.toPromise()
        }
        promise = promise.then(handler)
      },
      to: {
        promise: () => {
          if(promise === null){
            return observable.toPromise()
          }
          return promise
        },
        observable: () => observable
      }
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
    const errors = {
      conflicts: lockConflicts.map(
        conflict => ({
          command: conflict.instance.command.internal.name(),
          subsystem: conflict.subsystem
        })
      )
    }
    return {
      errors,
      to: {
        promise: () => Promise.reject(errors),
        observable: () => _throw(errors)
      }
    }
  }
  return {
    lockingFailed: false,
  }
}

const createSystemForCommand = (command, subsystems) => {
  const system = command.internal.requiredSubsystems().reduce((system, name) => {
    system[name] = subsystems.find(subsystem => subsystem.internal.name() == name).internal.raw()
    return system
  }, {})
  return system
}

const checkForMissingRequires = (command, subsystems) => {
  const missingRequires = command.internal.requiredSubsystems().filter(
    name => subsystems.findIndex(subsystem => subsystem.internal.name() == name) < 0
  )
  if(missingRequires.length > 0){
    const errors = {
      missing: missingRequires.map(
        missing => ({
          subsystem: missing
        })
      )
    }
    return {
      errors,
      to: {
        promise: () => Promise.reject(errors),
        observable: () => _throw(errors)
      }
    }
  }
  return {
    lockingFailed: false,
  }
}

const createControlForObservable = (observable, { onStart, onStop }) => {
  const stopSubject = new Subject()
  const publishedObservable = observable.pipe(takeUntil(stopSubject), publish())
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
        stopSubject.next()
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
