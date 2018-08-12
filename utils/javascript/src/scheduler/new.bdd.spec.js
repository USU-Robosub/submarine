const {
  given, when, then,
  Given, When, Then,
  Rules,
  unit,
} = require('jest-bdd')
const { Scheduler, Command } = require('./new')
const { interval, Observable, empty } = require('rxjs')
const { take, toArray, tap } = require('rxjs/operators')

const rules = Rules()



// ===========================================================
// ==                    Basic rules                        ==
// ===========================================================

// ===========================
//           Given
// ===========================

Given(rules.a_scheduler, () => {
  scope.scheduler = new Scheduler()
})

// ===========================
//            When
// ===========================

When(rules.the_command_is_run, () => {
  scope.instance = scheduler.run(command)
})

// ===========================
//            Then
// ===========================




// ===========================================================
// ==       Commands can import methods from modules        ==
// ===========================================================

// ===========================
//           Given
// ===========================

Given(rules.a_command_with_just_an_action, () => {
  scope.action = jest.fn()
  scope.command = new Command()
    .do(scope.action)
})

Given(rules.the_command_imports_a_method, () => {
  command.import('method').from('module')
})

Given(rules.the_command_imports_many_methods, () => {
  command
    .import('func1', 'func2').from('test')
    .import('func3').from('test')
    .import(['func4', 'func5']).from('test')
})

Given(rules.the_command_imports_methods_from_many_modules, () => {
  command
    .import('func').from('test1')
    .import('func1', 'func2').from('test2')
    .import('func').from('test3')
})

// ===========================
//            When
// ===========================

// ===========================
//            Then
// ===========================

Then(rules.the_action_is_called_once_with_an_empty_object, () => {
  expect(action).toHaveBeenCalledTimes(1)
  expect(action).toHaveBeenCalledWith({})
})

Then(rules.the_action_is_called_with_the_imported_method, () => {
  expect('module' in action.mock.calls[0][0]).toBeTruthy()
  expect('method' in action.mock.calls[0][0].module).toBeTruthy()
  expect(typeof action.mock.calls[0][0].module.method === "function").toBeTruthy()
})

Then(rules.the_action_is_called_with_the_imported_methods, () => {
  expect('test' in action.mock.calls[0][0]).toBeTruthy()
  expect('func1' in action.mock.calls[0][0].test).toBeTruthy()
  expect('func2' in action.mock.calls[0][0].test).toBeTruthy()
  expect('func3' in action.mock.calls[0][0].test).toBeTruthy()
  expect('func4' in action.mock.calls[0][0].test).toBeTruthy()
  expect('func5' in action.mock.calls[0][0].test).toBeTruthy()
})

Then(rules.the_action_is_called_with_the_imported_modules, () => {
  expect('test1' in action.mock.calls[0][0]).toBeTruthy()
  expect('func' in action.mock.calls[0][0].test1).toBeTruthy()

  expect('test2' in action.mock.calls[0][0]).toBeTruthy()
  expect('func1' in action.mock.calls[0][0].test2).toBeTruthy()
  expect('func2' in action.mock.calls[0][0].test2).toBeTruthy()

  expect('test3' in action.mock.calls[0][0]).toBeTruthy()
  expect('func' in action.mock.calls[0][0].test3).toBeTruthy()
})

// ===========================
//            Unit
// ===========================

rules.useWith(() => unit(
  given.a_scheduler(
    given.a_command_with_just_an_action(
      when.the_command_is_run(
        then.the_action_is_called_once_with_an_empty_object
      ),
      given.the_command_imports_a_method(
        when.the_command_is_run(
          then.the_action_is_called_with_the_imported_method
        )
      ),
      given.the_command_imports_many_methods(
        when.the_command_is_run(
          then.the_action_is_called_with_the_imported_methods
        )
      ),
      given.the_command_imports_methods_from_many_modules(
        when.the_command_is_run(
          then.the_action_is_called_with_the_imported_modules
        )
      )
    )
  )
))




// ===========================================================
// ==           Commands can lock methods/modules           ==
// ===========================================================

// ===========================
//           Given
// ===========================

Given(rules.two_forever_running_commands_that_import_the_same_method, () => {
  scope.firstCommand = new Command()
    .named('first')
    .import('method').from('module')
    .do(() => interval(1000)) // keep command alive forever

  scope.secondCommand = new Command()
    .named('second')
    .import('method').from('module')
    .do(() => interval(1000)) // keep command alive forever
})

Given(rules.the_module_is_shared, () => {
  scheduler.system.module.makeShared()
})

// ===========================
//            When
// ===========================

When(rules.the_first_command_is_run, () => {
  scope.firstInstance = scheduler.run(firstCommand)
})

When(rules.the_first_command_is_canceled, () => {
  firstInstance.cancel()
})

// ===========================
//            Then
// ===========================

Then(rules.the_second_command_will_throw_when_run, () => {
  expect(() => scheduler.run(secondCommand)).toThrow()
})

Then(rules.the_second_command_will_run_normally, () => {
  scheduler.run(secondCommand)
  expect(scheduler.runningByName().includes('second')).toBeTruthy()
})

// ===========================
//            Unit
// ===========================

rules.useWith(() => unit(
  given.a_scheduler(
    given.two_forever_running_commands_that_import_the_same_method(
      when.the_first_command_is_run(
        then.the_second_command_will_throw_when_run,
        when.the_first_command_is_canceled(
          then.the_second_command_will_run_normally
        )
      ),
      given.the_module_is_shared(
        when.the_first_command_is_run(
          then.the_second_command_will_run_normally
        )
      )
    )
  )
))



// ============================================================
// == Commands can by async (promise/observable) or not async ==
// ============================================================

// ===========================
//           Given
// ===========================

Given(rules.an_observable_based_command, () => {
  const commandState = {
    started: false,
    finished: false,
    finish: null
  }
  scope.commandState = commandState
  scope.command = new Command()
    .named('command')
    .import('method').from('module')
    .do(() => Observable.create(subscriber => {
      commandState.started = true
      subscriber.next(42)
      commandState.finish = () => {
        subscriber.complete()
      }
      return () => {
        commandState.finished = true
      }
    }))
})

Given(rules.a_non_async_command, () => {
  const commandState = {
    started: false,
    finished: false
  }
  scope.commandState = commandState
  scope.command = new Command()
    .named('command')
    .import('method').from('module')
    .do(() => {
      commandState.started = true
      // do action here in real command
      commandState.finished = true
      return 42
    })
})

Given(rules.a_promise_based_command, () => {
  const commandState = {
    started: false,
    finished: false,
    finish: null
  }
  scope.commandState = commandState
  scope.command = new Command()
    .named('command')
    .import('method').from('module')
    .do(() => {
      commandState.started = true
      return new Promise(resolve => {
        commandState.finish = () => {
          commandState.finished = true
          resolve(42)
        }
      })
    })
})

// ===========================
//            When
// ===========================

When(rules.the_command_is_canceled, () => {
  instance.cancel()
})

When(rules.the_command_finishes, () => {
  commandState.finish()
})

// ===========================
//            Then
// ===========================

Then(rules.the_command_is_running, () => {
  expect(scheduler.runningByName().includes('command')).toBeTruthy()
  expect(commandState.started).toBeTruthy()
})

Then(rules.the_command_is_finished_running, () => {
  expect(scheduler.runningByName().includes('command')).toBeFalsy()
  expect(commandState.finished).toBeTruthy()
})

Then(rules.all_locked_methods_are_unlocked, () => {
  expect(() => scheduler.run(
    new Command()
      .import('method').from('module')
      .do(() => {})
  )).not.toThrow()
})

Then(rules.the_command_instance_can_be_converted_to_an_observable, () => {
  return expect(instance.to.observable.toPromise()).resolves.toBe(42)
})

Then(rules.the_command_instance_can_be_converted_to_an_promise, () => {
  return expect(instance.to.promise).resolves.toBe(42)
})

Then(rules.the_command_instance_can_return_the_result, () => {
  return expect(instance.result).toBe(42)
})

// ===========================
//            Unit
// ===========================

rules.useWith(() => unit(
  given.a_scheduler(
    given.an_observable_based_command(
      when.the_command_is_run(
        then.the_command_is_running,
        when.the_command_finishes(
          then.the_command_is_finished_running,
          then.all_locked_methods_are_unlocked,
          then.the_command_instance_can_be_converted_to_an_observable,
          then.the_command_instance_can_be_converted_to_an_promise
        ),
        when.the_command_is_canceled(
          then.the_command_is_finished_running,
          then.all_locked_methods_are_unlocked,
          then.the_command_instance_can_be_converted_to_an_observable,
          then.the_command_instance_can_be_converted_to_an_promise
        )
      )
    ),
    given.a_non_async_command(
      when.the_command_is_run(
        then.the_command_is_finished_running,
        then.all_locked_methods_are_unlocked,
        then.the_command_instance_can_be_converted_to_an_observable,
        then.the_command_instance_can_be_converted_to_an_promise,
        then.the_command_instance_can_return_the_result
      )
    ),
    given.a_promise_based_command(
      when.the_command_is_run(
        then.the_command_is_running,
        when.the_command_finishes(
          then.the_command_is_finished_running,
          then.all_locked_methods_are_unlocked,
          then.the_command_instance_can_be_converted_to_an_observable,
          then.the_command_instance_can_be_converted_to_an_promise
        )
      )
    )
  )
))

/*

// ===========================================================
// ==       Commands can import methods from modules        ==
// ===========================================================

// ===========================
//           Given
// ===========================

// ===========================
//            When
// ===========================

// ===========================
//            Then
// ===========================

// ===========================
//            Unit
// ===========================

rules.useWith(() => unit(
  given.a_scheduler(

  )
))

*/
