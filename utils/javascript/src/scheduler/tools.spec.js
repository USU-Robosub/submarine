const { Scheduler, Subsystem, Command } = require('./index')
const { frc, sequential, concurrent } = require('./tools')
const { map, first, tap, delay, toArray } = require('rxjs/operators')
const { Subject, of } = require('rxjs')

const once = func => system => of(0).pipe(map(() => func(system)))
const delayed = (func, time=1) => system => once(func)(system).pipe(delay(time))

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
  const instance = Scheduler(subsystems).run(Command().require('subsystem').action(frc(action, {base})))

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

  expect(action.init).toHaveBeenCalledWith({subsystem})
  expect(action.execute).toHaveBeenCalledWith({subsystem})
  expect(action.isFinished).toHaveBeenCalledWith({subsystem})
  expect(action.end).toHaveBeenCalledWith({subsystem})
})

test('create command that runs other commands in sequence', () => {
  const scheduler = Scheduler()
  const base = new Subject()
  const instance = scheduler.run(
    sequential(
      Command().action(once(() => { console.log(1); return 1 })).named('1'),
      Command().action(once(() => { console.log(2); return 2 })).named('2'),
      Command().action(once(() => { console.log(3); return 3 })).named('3'),
    ).bind(scheduler).base(base).named('multiple')
  )
  const promise = instance.toObservable().pipe(toArray()).toPromise().then(result => {
    expect(result).toEqual([1, 2, 3])
  })
  // expect(scheduler.runningByName()).toEqual(['multiple'])
  // base.next()
  // expect(scheduler.runningByName()).toEqual(['multiple', '1'])
  // base.next()
  // expect(scheduler.runningByName()).toEqual(['multiple', '2'])
  // base.next()
  // //expect(scheduler.runningByName()).toEqual(['multiple', '3'])
  // base.next()
  //expect(scheduler.runningByName()).toEqual([])

  return promise
})

test('create command that runs other commands in concurrent', () => {
  const scheduler = Scheduler()
  const instance = scheduler.run(
    concurrent(
      Command().action(delayed(() => 1, 30)),
      Command().action(delayed(() => 2, 20)),
      Command().action(delayed(() => 3, 10)),
    ).named('multiple')
  )
  expect(scheduler.runningByName()).toEqual(['multiple'])
  return instance.toObservable().pipe(toArray()).toPromise().then(result => {
    expect(result).toEqual([3, 2, 1])
  })
})
