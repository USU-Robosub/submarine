const { Subsystem } = require('../scheduler')
const { Observable } = require('rxjs')
const { map, tap } = require('rxjs/operators')

const micro = 1000000

const attachToHubEvent = (hub, event) => Observable.create(observer => {
  const listener = hub.on(event, (hub, data) => {
    observer.next(data)
  })
  return () => {
    listener.remove()
  }
})

module.exports = (hub, handlerName="imu") => Subsystem()
  .named('imu')
  .makeShared()
  .raw({
    angularVelocity: () => attachToHubEvent(hub, handlerName + '/data').pipe(
      map(data => ({
        x: parseFloat(data[0]) / micro,
        y: parseFloat(data[1]) / micro,
        z: parseFloat(data[2]) / micro
      })),
      // tap(data => console.log('angularVelocity:', data))
    ),
    linearAcceleration: () => attachToHubEvent(hub, handlerName + '/data').pipe(
      map(data => ({
        x: parseFloat(data[3]) / micro,
        y: parseFloat(data[4]) / micro,
        z: parseFloat(data[5]) / micro
      }))
    ),
    megneticField: () => attachToHubEvent(hub, handlerName + '/data').pipe(
      map(data => ({
        x: parseFloat(data[6]),
        y: parseFloat(data[7]),
        z: parseFloat(data[8])
      }))
    ),
    // pressure: () => attachToHubEvent(hub, handlerName + '/data').pipe(
    //   map(data => parseFloat(data[9]))
    // ),
    // temperature: () => attachToHubEvent(hub, handlerName + '/data').pipe(
    //   map(data => parseFloat(data[10]))
    // ),
  })
