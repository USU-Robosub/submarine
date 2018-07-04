const Scheduler = require('./index')
const { tap, finalize } = require('rxjs/operators')
const { pipe } = require('rxjs')

test('happy path', done => {
  const scheduler = Scheduler()
  const command = scheduler.run({
    name: 'test',
    requires: [],
    action: () => pipe(
      tap(x => {
        console.log(x)
        command.cancel()
      }),
      finalize(() => {
        console.log('finalize command')
        done()
      })
    )
  })
})

test('throw if not given a command', () => {
  expect(() => Scheduler().run()).toThrowError('Must pass a command')
})

test('throw if given a command without requires', () => {
  expect(() => Scheduler().run({
    name: 'test',
    action: () => {}
  })).toThrowError('Command must require an array of subsystems')
})

test('throw if given a command without action', () => {
  expect(() => Scheduler().run({
    name: 'test',
    requires: []
  })).toThrowError('Command must have an action to perform')
})

test('throw if given a command without a name', () => {
  expect(() => Scheduler().run({
    requires: [],
    action: () => {}
  })).toThrowError('Command must have a name')
})

test('locks an actuator subsystem so only one command can use it', done => {
  const system = {
    actuator: {
      mustBeLocked: true,
      commands: {
        inactive: null,
      },
      method: () => {}
    }
  }
  const scheduler = Scheduler(system)
  const command = scheduler.run({
    name: 'test',
    requires: ['actuator'],
    action: () => pipe(
      tap(() => command.cancel()),
      finalize(() => done())
    )
  })
  expect(() => scheduler.run({
    name: 'test2',
    requires: ['actuator'],
    action: () => {}
  })).toThrowError('Command \"test2\" attempted to lock subsystem \"actuator\" when it is already locked by command \"test\"')
})

test('does not lock a sensor subsystem', done => {
  const system = {
    actuator: {
      mustBeLocked: false,
      commands: {
        inactive: null,
      },
      method: () => {}
    }
  }
  const scheduler = Scheduler(system)
  const command = scheduler.run({
    name: 'test',
    requires: ['actuator'],
    action: () => pipe(
      tap(() => command.cancel()),
      finalize(() => done())
    )
  })
  expect(() => scheduler.run({
    name: 'test2',
    requires: ['actuator'],
    action: () => pipe()
  })).not.toThrow()
})

test('required subsystems are injected into the command\'s action', done => {
  const system = {
    actuator: {
      mustBeLocked: true,
      commands: {
        inactive: null,
      },
      method: () => {}
    }
  }
  const command = Scheduler(system).run({
    name: 'test',
    requires: ['actuator'],
    action: required => pipe(
      tap(() => {
        expect(required.actuator).toBe(system.actuator)
        command.cancel()
      }),
      finalize(() => done())
    )
  })
})
