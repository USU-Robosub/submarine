const Scheduler = require('./index')
const { convertFRC_LikeCommand, sequential, concurrent } = require('./tools')

test('frc like command', () => {
  const system = {subsystem1: 0, subsystem2: 1, subsystem3: 2}
  const FRC_likeCommand = {
    requires: ['subsystem1', 'subsystem3'],
    init: jest.fn(),
    execute: jest.fn(),
    isFinished: jest.fn()
      .mockReturnValueOnce(false)
      .mockReturnValueOnce(false)
      .mockReturnValueOnce(true),
    end: jest.fn(),
  }
  const instance = Scheduler(system).run(convertFRC_LikeCommand('command name', FRC_likeCommand), {manual: true})

  expect(FRC_likeCommand.init).toHaveBeenCalledTimes(0)
  expect(FRC_likeCommand.execute).toHaveBeenCalledTimes(0)
  expect(FRC_likeCommand.isFinished).toHaveBeenCalledTimes(0)
  expect(FRC_likeCommand.end).toHaveBeenCalledTimes(0)
  instance.step()
  expect(FRC_likeCommand.init).toHaveBeenCalledTimes(1)
  expect(FRC_likeCommand.execute).toHaveBeenCalledTimes(0)
  expect(FRC_likeCommand.isFinished).toHaveBeenCalledTimes(1)
  expect(FRC_likeCommand.end).toHaveBeenCalledTimes(0)
  instance.step()
  expect(FRC_likeCommand.init).toHaveBeenCalledTimes(1)
  expect(FRC_likeCommand.execute).toHaveBeenCalledTimes(1)
  expect(FRC_likeCommand.isFinished).toHaveBeenCalledTimes(2)
  expect(FRC_likeCommand.end).toHaveBeenCalledTimes(0)
  instance.step()
  expect(FRC_likeCommand.init).toHaveBeenCalledTimes(1)
  expect(FRC_likeCommand.execute).toHaveBeenCalledTimes(2)
  expect(FRC_likeCommand.isFinished).toHaveBeenCalledTimes(3)
  expect(FRC_likeCommand.end).toHaveBeenCalledTimes(1)

  const requires = {subsystem1: system.subsystem1, subsystem3: system.subsystem3}
  expect(FRC_likeCommand.init).toHaveBeenCalledWith(requires)
  expect(FRC_likeCommand.execute).toHaveBeenCalledWith(requires)
  expect(FRC_likeCommand.isFinished).toHaveBeenCalledWith(requires)
  expect(FRC_likeCommand.end).toHaveBeenCalledWith(requires)
})
//
// test('can combine commands', done => {
//   let count = 0
//   const test = {
//     requires: [],
//     init: requires => console.log('FRC init'),
//     execute: requires => {
//       console.log('FRC execute')
//       count++
//       return 'hello'
//     },
//     isFinished: requires => {
//       console.log('FRC isFinished')
//       return count > 5
//     },
//     end: requires => {
//       console.log('FRC end')
//     },
//   }
//   const scheduler = Scheduler()
//   const instance = scheduler.run(convertFRC_LikeCommand('test', test))
//   instance.toPromise().then(() => {
//     console.log('Done!')
//     done()
//   })
//   // instance.pipe(
//   //   tap(x => console.log('Tap', x))
//   // ).subscribe(() => console.log('Next'), e => console.log('Error', e), () => console.log('Complete'))
// })
//
// // const hotSwap = currentOperator => source => new Observable(observer => {
// //   let count = 0;
// //   return source.subscribe({
// //     next(x) {
// //       currentOperator()(source)
// //     },
// //     error(err) { observer.error(err); },
// //     complete() { observer.complete(); }
// //   })
// // })
//
//
//
// test.only('multiple commands in one', () => {
//   try{
//   let count = 0
//   const scheduler = Scheduler()
//   const doSomething = name => {
//     let count = 0
//     return {
//       requires: [],
//       execute: () => {
//         count++
//         //console.log(name)
//       },
//       isFinished: () => count > 5,
//     }
//   }
//   setTimeout(() => {
//     try{
//     console.log(tree(scheduler.running()))
//   }catch(e){
//     console.log(e)
//   }
// }, 1000)
//   scheduler.run(convertFRC_LikeCommand('something else', doSomething('something else')), {interval: 1000})
//   return scheduler.run(sequential('score goal',
//     convertFRC_LikeCommand('aim', doSomething('test')),
//     concurrent('kick ball',
//       convertFRC_LikeCommand('spin kicker', doSomething('test')),
//       convertFRC_LikeCommand('open hatch', doSomething('test2'))
//     )
//   ), {interval: 100}).toPromise().then(() => {
//     console.log('Done!')
//   })
// }catch(e){
//   console.error('Error!!!!', e)
//   return Promise.resolve()
// }
// })
