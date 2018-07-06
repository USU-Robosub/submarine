const { Subsystem, Command, Scheduler } = require('./next')
const { map, first, tap, delay } = require('rxjs/operators')
const { of, interval, empty } = require('rxjs')

const once = func => system => of(0).pipe(map(() => func(system)))
const forever = system => interval(1000)
const nothing = system => empty()

test('runs very simple command', () => {
  const mockFn = jest.fn()
  return Scheduler().run(
    Command().action(once(mockFn))
  ).toPromise().then(() => {
    expect(mockFn).toHaveBeenCalledTimes(1)
    expect(mockFn).toHaveBeenCalledWith({})
  })
})

test('runs command that requires a subsystem', () => {
  const test = {
    method: () => {}
  }
  const subsystems = [
    Subsystem(test).named('test')
  ]
  const mockFn = jest.fn()
  return Scheduler(subsystems).run(
    Command().require('test').action(once(mockFn))
  ).toPromise().then(() => {
    expect(mockFn).toHaveBeenCalledWith({test})
  })
})

test('only one command can use a subsystem by default', () => {
  const test = {
    method: () => {}
  }
  const subsystems = [
    Subsystem(test).named('test')
  ]
  const scheduler = Scheduler(subsystems)
  scheduler.run(Command().require('test').action(forever).named('first'))
  expect(scheduler.run(Command().require('test').action(forever)))
    .toEqual({lockingFailed: true, lockConflicts: [{command: 'first', subsystem: 'test'}]})
})

test('multiple commands can use a shared subsystem', () => {
  const test = {
    method: () => {}
  }
  const subsystems = [
    Subsystem(test).named('test').makeShared()
  ]
  const scheduler = Scheduler(subsystems)
  scheduler.run(Command().require('test').action(forever).named('first'))
  scheduler.run(Command().require('test').action(forever).named('second'))
  expect(scheduler.runningByName()).toEqual(['first', 'second'])
})

test('subsystems unlock after command completes', () => {
  const test = {
    method: () => {}
  }
  const subsystems = [
    Subsystem(test).named('test')
  ]
  const scheduler = Scheduler(subsystems)
  const first = scheduler.run(Command().require('test').action(forever).named('first'))
  expect(scheduler.runningByName()).toEqual(['first'])
  first.abort()
  const second = scheduler.run(Command().require('test').action(forever).named('second'))
  expect(scheduler.runningByName()).toEqual(['second'])
})

test('immediate complete actions are still removed from instances', () => {
  const scheduler = Scheduler()
  scheduler.run(Command().action(nothing))
  expect(scheduler.runningByName()).toEqual([])
})

test('can abort any command', () => {
  const scheduler = Scheduler()
  const first = scheduler.run(Command().action(forever).named('test'))
  expect(scheduler.runningByName()).toEqual(['test'])
  first.abort()
  expect(scheduler.runningByName()).toEqual([])
})

test('can cancel cancelable command', () => {
  const scheduler = Scheduler()
  const first = scheduler.run(Command().action(forever).named('test').makeCancelable())
  expect(scheduler.runningByName()).toEqual(['test'])
  expect(first.cancel()).toBeTruthy()
  expect(scheduler.runningByName()).toEqual([])
})

test('cannot cancel normal command', () => {
  const scheduler = Scheduler()
  const first = scheduler.run(Command().action(forever).named('test'))
  expect(scheduler.runningByName()).toEqual(['test'])
  expect(first.cancel()).toBeFalsy()
  expect(scheduler.runningByName()).toEqual(['test'])
})

test('subsystems can have a command that is run when no other command is run', () => {
  const scheduler = Scheduler([
    Subsystem().whenIdleRun(Command().action(forever).named('test'))
  ])
  expect(scheduler.runningByName()).toEqual(['test'])
})

test('a subsystem\'s default command is started after it is unlocked', () => {
  const scheduler = Scheduler([
    Subsystem().named('subsystem').whenIdleRun(
      Command().require('subsystem').action(forever).named('default').makeCancelable()
    )
  ])
  expect(scheduler.runningByName()).toEqual(['default'])
  const instance = scheduler.run(Command().require('subsystem').action(forever).named('test'))
  expect(scheduler.runningByName()).toEqual(['test'])
  instance.abort()
  expect(scheduler.runningByName()).toEqual(['default'])
})

test('a shareable subsystem\'s default command is always running', () => {
  const scheduler = Scheduler([
    Subsystem().named('subsystem').makeShared().whenIdleRun(
      Command().require('subsystem').action(forever).named('default').makeCancelable()
    )
  ])
  expect(scheduler.runningByName()).toEqual(['default'])
  const instance = scheduler.run(Command().require('subsystem').action(forever).named('test'))
  expect(scheduler.runningByName()).toEqual(['default', 'test'])
  instance.abort()
  expect(scheduler.runningByName()).toEqual(['default'])
})

test('cancels cancelable command when new command needs a subsystem', () => {
  const scheduler = Scheduler([
    Subsystem().named('subsystem')
  ])
  expect(scheduler.runningByName()).toEqual([])
  const first = scheduler.run(Command().require('subsystem').action(forever).named('first').makeCancelable())
  expect(scheduler.runningByName()).toEqual(['first'])
  const second = scheduler.run(Command().require('subsystem').action(forever).named('second'))
  expect(scheduler.runningByName()).toEqual(['second'])
})

test('can convert command instance to promise', () => {
  return Scheduler().run(
    Command().action(() => of(42).pipe(delay(1)))
  ).toPromise().then(result => {
    expect(result).toBe(42)
  })
})

test('can convert command instance to observable', () => {
  return Scheduler().run(
    Command().action(() => of(42).pipe(delay(1)))
  ).toObservable().pipe(
    tap(result => expect(result).toBe(42))
  ).toPromise()
})
