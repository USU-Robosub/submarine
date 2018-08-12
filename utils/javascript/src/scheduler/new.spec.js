const { Scheduler, Command } = require('./new')
const { interval, Observable, empty } = require('rxjs')
const { take, toArray } = require('rxjs/operators')

const forever = system => interval(1000)

test('sandbox', () => {
  const scheduler = new Scheduler()
  scheduler.system.module.group('method', 'other')
  return expect(scheduler.run(new Command()
    .named('first')
    .import('func').from('test')
    .do(() => 42)
  ).to.observable.pipe(toArray()).toPromise()).resolves.toEqual([42])
})
