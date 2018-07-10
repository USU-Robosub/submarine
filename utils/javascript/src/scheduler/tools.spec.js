const { Scheduler, Subsystem, Command } = require('./index')
const { frc, sequential, concurrent, once, delayThen, repeat, repeatWithDelay, fromPromise } = require('./tools')
const { map, first, tap, delay, toArray, take } = require('rxjs/operators')
const { Subject, of, from } = require('rxjs')

test('once() runs the predicate a single time', () => {
  const fn = jest.fn()
  return Scheduler().build(
    Command().action(once(() => 42))
  ).then(result => {
    expect(result).toBe(42)
  }).run().toPromise()
})

test('delayThen() runs the predicate a single time after a delay', () => {
  const fn = jest.fn()
  return Scheduler().build(
    Command().action(delayThen(() => 42, 10))
  ).run().toPromise().then(result => {
    expect(result).toBe(42)
  })
})

test('repeat() runs the predicate repeatedly forever', () => {
  const fn = jest.fn()
  return Scheduler().build(
    Command().action(repeat(() => 42))
  ).run().toObservable().pipe(take(5), toArray()).toPromise().then(result => {
    expect(result).toEqual([42, 42, 42, 42, 42])
  })
})

test('repeatWithDelay() runs the predicate repeatedly forever with a delay between runs', () => {
  const fn = jest.fn()
  return Scheduler().build(
    Command().action(repeatWithDelay(() => 42, 10))
  ).run().toObservable().pipe(take(5), toArray()).toPromise().then(result => {
    expect(result).toEqual([42, 42, 42, 42, 42])
  })
})

test('fromPromise() converts promise into a command action', () => {
  const fn = jest.fn()
  return Scheduler().build(
    Command().action(fromPromise(() => Promise.resolve(42)))
  ).run().toPromise().then(result => {
    expect(result).toBe(42)
  })
})

test('sequence commands run one after another', () => {
  const order = []
  const instance = Scheduler().run(
    sequential(
      Command().action(delayThen(() => { order.push(1); return 1 }, 30)),
      Command().action(delayThen(() => { order.push(2); return 2 }, 20)),
      Command().action(delayThen(() => { order.push(3); return 3 }, 10)),
    )
  )
  return instance.toObservable().pipe(toArray()).toPromise().then(result => {
    expect(result).toEqual([1, 2, 3])
    expect(order).toEqual([1, 2, 3])
  })
})

test('concurrent commands run at the same time', () => {
  const order = []
  const instance = Scheduler().run(
    concurrent(
      Command().action(delayThen(() => { order.push(1); return 1 }, 30)),
      Command().action(delayThen(() => { order.push(2); return 2 }, 20)),
      Command().action(delayThen(() => { order.push(3); return 3 }, 10)),
    )
  )
  return instance.toObservable().pipe(toArray()).toPromise().then(result => {
    expect(result).toEqual([3, 2, 1])
    expect(order).toEqual([3, 2, 1])
  })
})

test('create command that uses the same interface as FRC', () => {
  const subsystem = {
    method: () => {}
  }
  const subsystems = [
    Subsystem(subsystem).named('subsystem')
  ]
  const action = {
    init: jest.fn()
      .mockReturnValue(1),
    execute: jest.fn()
      .mockReturnValue(2),
    isFinished: jest.fn()
      .mockReturnValueOnce(false)
      .mockReturnValueOnce(false)
      .mockReturnValueOnce(true),
    end: jest.fn(),
  }
  return Scheduler(subsystems).run(
    Command().require('subsystem').action(frc(action))
  ).toPromise().then(result => {
    expect(result).toEqual([1, 2, 2])

    expect(action.init).toHaveBeenCalledTimes(1)
    expect(action.execute).toHaveBeenCalledTimes(2)
    expect(action.isFinished).toHaveBeenCalledTimes(3)
    expect(action.end).toHaveBeenCalledTimes(1)

    expect(action.init).toHaveBeenCalledWith({subsystem})
    expect(action.execute).toHaveBeenCalledWith({subsystem})
    expect(action.isFinished).toHaveBeenCalledWith({subsystem})
    expect(action.end).toHaveBeenCalledWith({subsystem})
  })
})

test('create command that uses the same interface as FRC', () => {
  const subsystem = {
    method: () => {}
  }
  const subsystems = [
    Subsystem(subsystem).named('subsystem')
  ]
  const action = {
    init: jest.fn(),
    execute: jest.fn(),
    isFinished: jest.fn()
      .mockReturnValueOnce(false)
      .mockReturnValueOnce(false)
      .mockReturnValueOnce(true),
    end: jest.fn(),
  }
  const base = new Subject()
  const instance = Scheduler(subsystems).run(
    Command().require('subsystem').action(frc(action, {base}))
  )

  expect(action.init).toHaveBeenCalledTimes(0)
  expect(action.execute).toHaveBeenCalledTimes(0)
  expect(action.isFinished).toHaveBeenCalledTimes(0)
  expect(action.end).toHaveBeenCalledTimes(0)
  base.next()
  expect(action.init).toHaveBeenCalledTimes(1)
  expect(action.execute).toHaveBeenCalledTimes(0)
  expect(action.isFinished).toHaveBeenCalledTimes(1)
  expect(action.end).toHaveBeenCalledTimes(0)
  base.next()
  expect(action.init).toHaveBeenCalledTimes(1)
  expect(action.execute).toHaveBeenCalledTimes(1)
  expect(action.isFinished).toHaveBeenCalledTimes(2)
  expect(action.end).toHaveBeenCalledTimes(0)
  base.next()
  expect(action.init).toHaveBeenCalledTimes(1)
  expect(action.execute).toHaveBeenCalledTimes(2)
  expect(action.isFinished).toHaveBeenCalledTimes(3)
  expect(action.end).toHaveBeenCalledTimes(1)
})

test('sequence allows a custom base observable', () => {
  const scheduler = Scheduler()
  const base = new Subject()
  const allowNextSequential = () => base.next()
  const inner = new Subject()
  const stopCurrentInnerCommand = () => inner.next()
  const instance = scheduler.run(
    sequential(
      Command().action(() => inner.pipe(take(1), map(() => 1))).named('1'),
      Command().action(() => inner.pipe(take(1), map(() => 2))).named('2'),
      Command().action(() => inner.pipe(take(1), map(() => 3))).named('3'),
    ).base(base).named('multiple')
  )
  const promise = instance.toObservable().pipe(toArray()).toPromise().then(result => {
    expect(result).toEqual([1, 2, 3])
  })
  expect(scheduler.runningByName()).toEqual(['multiple'])
  allowNextSequential()
  expect(scheduler.runningByName()).toEqual(['multiple', '1'])
  stopCurrentInnerCommand()
  allowNextSequential()
  expect(scheduler.runningByName()).toEqual(['multiple', '2'])
  stopCurrentInnerCommand()
  allowNextSequential()
  expect(scheduler.runningByName()).toEqual(['multiple', '3'])
  stopCurrentInnerCommand()
  expect(scheduler.runningByName()).toEqual([])

  return promise
})

test('concurrent allows a custom base observable', () => {
  const scheduler = Scheduler()
  const base = new Subject()
  const allowNextSequential = () => base.next()
  const inner = new Subject()
  const stopCurrentInnerCommand = () => inner.next()
  const instance = scheduler.run(
    concurrent(
      Command().action(() => inner.pipe(take(1), map(() => 1))).named('1'),
      Command().action(() => inner.pipe(take(1), map(() => 2))).named('2'),
      Command().action(() => inner.pipe(take(1), map(() => 3))).named('3'),
    ).base(base).named('multiple')
  )
  const promise = instance.toObservable().pipe(toArray()).toPromise().then(result => {
    expect(result).toEqual([1, 2, 3])
  })
  expect(scheduler.runningByName()).toEqual(['multiple'])
  allowNextSequential()
  expect(scheduler.runningByName()).toEqual(['multiple', '1', '2', '3'])
  stopCurrentInnerCommand()
  expect(scheduler.runningByName()).toEqual([])

  return promise
})
