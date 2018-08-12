const { takeUntil, publish, finalize, tap, catchError, publishReplay } = require('rxjs/operators')
const { Observable, Subject, from } = require('rxjs')

class Command {
  constructor() {
    this.internal = {
      performAction: () => console.error('Warning: Running a command that does not have an action.'),
      imports: [],
      name: '<anonymous>'
    }
  }

  do(action){
    this.internal.performAction = action
    return this
  }

  import(...args){
    const methods = mergeIntoOneArray(args)
    return {
      from: (module) => {
        const moduleIndex = this.internal.imports.findIndex(
          item => item.module === module
        )
        if(moduleIndex >= 0){
          this.internal.imports[moduleIndex].methods.push(...methods)
        }else{
          this.internal.imports.push({module, methods})
        }
        return this
      }
    }
  }

  named(name){
    this.internal.name = name
    return this
  }
}

class Scheduler{
  constructor() {
    this.instances = []
    this.modules = []
    this.system = new Proxy({}, {
      get: (target, prop) => {
        return {
          makeShared: () => {
            this.modules.push({module: prop, shared: true})
          }
        }
      }
    })
  }

  runningByName(){
    return this.instances.map(instance => instance.name)
  }

  run(command){
    assertNoLockConflicts(command, this.instances, this.modules)

    const instance = {
      locks: command.internal.imports.reduce(
        (array, item) => {
          array.push(...item.methods.map(method => ({module: item.module, method})))
          return array
        }
      , []),
      name: command.internal.name
    }
    this.instances.push(instance)
    const removeLocks = () => this.instances.splice(this.instances.indexOf(instance), 1)

    const imports = command.internal.imports.reduce((obj, item) => {
      obj[item.module] = {}
      item.methods.forEach(method => obj[item.module][method] = () => {})
      return obj
    }, {})
    const actionReturn = command.internal.performAction(imports)

    let cancelObservable = () => console.error('Tried to cancel a command that does not support canceling.')
    let commandAsPromise = null
    let commandAsObservable = null
    let commandAsResult = null
    if(actionReturn && actionReturn instanceof Observable){
      // the action uses an observable to be async
      const stopSubject = new Subject()
      const observable = actionReturn.pipe(
        takeUntil(stopSubject),
        finalize(() => removeLocks()),
        publishReplay(),
      )
      observable.connect()
      commandAsObservable = observable
      commandAsPromise = observable.toPromise()
      cancelObservable = () => stopSubject.next()
    }else if(actionReturn && actionReturn instanceof Promise){
      // the action uses a promise to be async
      commandAsPromise = actionReturn.then(result => {
        removeLocks()
        return result
      }).catch(err => {
        removeLocks()
        throw Error(err)
      })
      commandAsObservable = from(commandAsPromise)
    }else{
      // the action is not async
      removeLocks() // remove locks imediatly after running the action
      commandAsPromise = Promise.resolve(actionReturn)
      commandAsObservable = from(commandAsPromise)
      commandAsResult = actionReturn
    }

    return {
      cancel: cancelObservable,
      to: {
        promise: commandAsPromise,
        observable: commandAsObservable
      },
      result: commandAsResult
    }
  }
}

module.exports = {
  Command,
  Scheduler
}

const assertNoLockConflicts = (command, instances, modules) => {
  const conflicts = mergeIntoOneArray(command.internal.imports.map(
    item => item.methods.map(
      method => instances.map(
        instance => instance.locks.find(
          lock => lock.module === item.module && lock.method === method
        )
      )
    )
  )).filter(ignoreSharedModules(modules))
  if(conflicts.length > 0){
    throw Error('Failed to run command because it imported a method already in use.')
  }
}

const ignoreSharedModules = modules => conflict =>
  !(modules.find(item => item.module == conflict.module))

const mergeIntoOneArray = arrayOfThings => arrayOfThings.reduce((array, thing) => {
  if(Array.isArray(thing)){
    return array.concat(mergeIntoOneArray(thing))
  }else{
    array.push(thing)
    return array
  }
}, [])
