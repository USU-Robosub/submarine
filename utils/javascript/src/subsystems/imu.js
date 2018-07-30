const { Subsystem } = require('../scheduler')
const { map, publish, refCount } = require('rxjs/operators')
const { tools:{ listenToHub } } = require('../comm')

const micro = 1000000

module.exports = (hub, handlerName="imu") => {
  const imuDataObservable = listenToHub(hub, handlerName + '/data')
  const basePressure = 0
  const baseTemp = 0
  return {
    subsystem: Subsystem()
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
        rawPressure: () => imuDataObservable.pipe(
          map(data => parseFloat(data[9])),
          publish(),
          refCount()
        ),
        temperature: () => imuDataObservable.pipe(
          map(data => parseFloat(data[10])),
          publish(),
          refCount()
        ),
        setPressureConfig: (pressureOffset, tempOffset) => {
          basePressure += pressureOffset
          baseTemp = tempOffset
        },
        pressure: () => imuDataObservable.pipe(
          map(data => {
            const raw = parseFloat(data[9])
            const temp = parseFloat(data[10])
            const corrected = raw - ((basePressure * temp) / baseTemp)
            return corrected
          }),
          publish(),
          refCount()
        ),
      }),
    setPressureConfigToCurrent: Command()
      .named('set pressure to current')
      .require('imu')
      .action(system => {
        return zip(
          system.imu.pressure.pipe(
            take(1)
          ),
          system.imu.temperature.pipe(
            take(1)
          )
        ).pipe(
          map([ pressure, temperature ] => {
            system.imu.setPressureConfig(pressure, temperature)
          })
        )
      })
}
