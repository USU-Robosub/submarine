const { Subsystem } = require('../scheduler')
const { map, publish, refCount } = require('rxjs/operators')
const { tools:{ listenToHub } } = require('../comm')

const micro = 1000000

module.exports = (hub, handlerName="imu") => {
  const imuDataObservable = listenToHub(hub, handlerName + '/data')
  return Subsystem()
    .named('imu')
    .makeShared()
    .raw({
      angularVelocity: () => imuDataObservable.pipe(
        map(data => ({
          x: parseFloat(data[0]) / micro,
          y: parseFloat(data[1]) / micro,
          z: parseFloat(data[2]) / micro
        })),
        publish(),
        refCount()
      ),
      linearAcceleration: () => imuDataObservable.pipe(
        map(data => ({
          x: parseFloat(data[3]) / micro,
          y: parseFloat(data[4]) / micro,
          z: parseFloat(data[5]) / micro
        })),
        publish(),
        refCount()
      ),
      magneticField: () => imuDataObservable.pipe(
        map(data => ({
          x: parseFloat(data[6]),
          y: parseFloat(data[7]),
          z: parseFloat(data[8])
        })),
        publish(),
        refCount()
      ),
      pressure: () => imuDataObservable.pipe(
        map(data => parseFloat(data[9])),
        publish(),
        refCount()
      ),
      temperature: () => imuDataObservable.pipe(
        map(data => parseFloat(data[10])),
        publish(),
        refCount()
      ),
    })
}