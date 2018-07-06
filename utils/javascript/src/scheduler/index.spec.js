const Scheduler = require('./index')
const { tap, finalize, first, skip } = require('rxjs/operators')
const { pipe, merge } = require('rxjs')

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

test('cancels old command if cancelable and subsystem is not shareable', done => {
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
    cancelable: true,
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

test('can only cancel cancelable commands', () => {
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
    cancelable: false,
    requires: ['actuator'],
    action: () => pipe(
      finalize(() => {})
    )
  })
  expect(() => command.cancel()).toThrowError('Can only cancel a command marked as cancelable. The command \"test\" is not marked cancelable')
})

test('can abort non cancelable commands', (done) => {
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
    cancelable: false,
    requires: ['actuator'],
    action: () => pipe(
      finalize(() => done())
    )
  })
  expect(() => command.abort()).not.toThrow()
})

test('some subsystems can be shared by commands', done => {
  const system = {
    sensor: {
      mustBeLocked: false,
      commands: {
        inactive: null,
      },
      method: () => {}
    },
    sensor2: {
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
    requires: ['sensor'],
    action: () => pipe(
      finalize(() => {})
    )
  })
  const command2 = scheduler.run({
    name: 'test2',
    requires: ['sensor', 'sensor2'],
    action: () => pipe(
      finalize(() => done())
    )
  })
  command.abort()
  expect(() => scheduler.run({
    name: 'test3',
    requires: ['sensor2'],
    action: () => pipe(
      finalize(() => {})
    )
  })).toThrowError('Command \"test3\" attempted to lock subsystem \"sensor2\" when it is already locked by command \"test2\"')
  command2.abort()
})

test('promise based command', () => {
  const system = {subsystem1: 0, subsystem2: 1, subsystem3: 2}
  const requires = {subsystem1: system.subsystem1, subsystem3: system.subsystem3}
  const promiseBasedCommand = {
    name: 'test',
    cancelable: true,
    requires: ['subsystem1', 'subsystem3'],
    action: jest.fn().mockReturnValueOnce(Promise.resolve(42))
  }
  const instance = Scheduler(system).run(promiseBasedCommand)
  expect(instance.cancelable).toBeFalsy()
  return instance.toPromise().then(result => {
    expect(result).toBe(42)
  })
})
