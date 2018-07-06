const { Scheduler, Command } = require('./index')
const { tap, finalize, first, skip, toArray, delay } = require('rxjs/operators')
const { pipe, merge, of, timer } = require('rxjs')

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

test('Command base: promise', () => {
  return Scheduler().run(
    Command('test', [], jest.fn().mockReturnValue(Promise.resolve(42)))
  ).toPromise().then(result => {
    expect(result).toBe(42)
  })
})

test('Command base: custom observable', () => {
  return Scheduler().run(
    Command('test', [], jest.fn().mockReturnValue(of(42)))
  ).toPromise().then(result => {
    expect(result).not.toBeDefined()
  })
})

test('Command base: custom observable, can be started manualy', () => {
  const instance = Scheduler().run(
    Command('test', [], jest.fn().mockReturnValue(of(42))
  ), {manual: true})
  const promise = instance.toPromise().then(result => {
    expect(result).toBe(42)
  })
  instance.step()
  return promise
})

test('subsystems can have default commands', () => {
  const system = {
    sensor: {
      mustBeLocked: true,
      commands: {
        default: Command('test', ['sensor'], () => pipe(), {cancelable: true}),
      },
      method: () => {}
    }
  }
  const scheduler = Scheduler(system)
  expect(scheduler.running()).toEqual(['test'])
})

test('subsystems default commands are started after they are unlocked', () => {
  const system = {
    sensor: {
      mustBeLocked: true,
      commands: {
        default: Command('default', ['sensor'], () => pipe(), {cancelable: true}),
      },
      method: () => {}
    }
  }
  const scheduler = Scheduler(system)
  expect(scheduler.running()).toEqual(['default'])
  const instance = scheduler.run(
    Command('other', ['sensor'], () => pipe())
  )
  expect(scheduler.running()).toEqual(['other'])
  instance.abort()
  expect(scheduler.running()).toEqual(['default'])
})

test('subsystems default commands always run if can be shared', () => {
  const system = {
    sensor: {
      commands: {
        default: Command('default', ['sensor'], () => pipe(), {cancelable: true}),
      },
      method: () => {}
    }
  }
  const scheduler = Scheduler(system)
  expect(scheduler.running()).toEqual(['default'])
  const instance = scheduler.run(
    Command('other', ['sensor'], () => pipe())
  )
  expect(scheduler.running()).toEqual(['default', 'other'])
  instance.abort()
  expect(scheduler.running()).toEqual(['default'])
})

test('command unlocks subsystems when canceled or aborted', () => {
  const system = {
    sensor: {
      mustBeLocked: true,
      method: () => {}
    }
  }
  const scheduler = Scheduler(system)
  expect(scheduler.running()).toEqual([])
  const instance = scheduler.run(
    Command('other', ['sensor'], () => pipe())
  )
  expect(scheduler.running()).toEqual(['other'])
  instance.abort()
  expect(scheduler.running()).toEqual([])
})
