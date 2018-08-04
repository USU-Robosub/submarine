const { Subsystem, Command } = require('../scheduler')
const { timeInterval, map, tap, withLatestFrom, publish, refCount, pairwise } = require('rxjs/operators')
const { zip, empty, Subject } = require('rxjs')
const { geometry } = require('../math')
const math = require('mathjs')

const x = 0, y = 1, z = 2

module.exports = (hub, handlerName="pose") => {
  
  let enabled = false;
  
  let accelLerpAmount = 0.01;
  let magnetLerpAmount = 0.1;
  
  let downNorthObservable = empty()
  
  let lastYawVelocity = [];
  
  let lastYaw = [];
  
  return Subsystem()
    .named('pose')
    .makeShared()
    .raw({
      enable: () => {
        enabled = true
      },
      disable: () => {
        enabled = false
      },
      setAccelLerpAmount: amount => {
        accelLerpAmount = amount
      },
      setMagnetLerpAmount: amount => {
        magnetLerpAmount = amount
      },
      yaw: () => {
        return downNorthObservable.pipe(
          map(([ down, north ]) => {
            let newYaw = geometry.findAngleInPlane(down, north, [1, 0, 0])
            if(lastYaw.length > 10){
              lastYaw.shift()
            }
            lastYaw.push(newYaw)
            const averageYaw = lastYaw.reduce((sum, item) => sum + item) / lastYaw.length
            console.log('Heading', averageYaw)
            return averageYaw
          })
        )
      },
      yawVelocity: () => {
        return downNorthObservable.pipe(
          pairwise(),
          timeInterval(),
          map(({ value: [[ lastDown, lastNorth ], [ down, north ]], interval: deltaTime }) => {
            const oldYaw = geometry.findAngleInPlane(lastDown, lastNorth, [1, 0, 0])
            const yaw = geometry.findAngleInPlane(down, north, [1, 0, 0])
            let newYawVelocity = ((yaw - oldYaw) / (deltaTime * 0.001))
            if(lastYawVelocity.length > 10){
              lastYawVelocity.shift()
            }
            lastYawVelocity.push(newYawVelocity)
            const averageYawVelocity = lastYawVelocity.reduce((sum, item) => sum + item) / lastYawVelocity.length
            return averageYawVelocity
          }),
        )
      },
      pitch: () => {
        return downNorthObservable.pipe(
          map(([ down, north ]) => {
            return geometry.findAngleInPlane([0, 1, 0], [0, 0, 1], down)
          })
        )
      },
      roll: () => {
        return downNorthObservable.pipe(
          map(([ down, north ]) => {
            return geometry.findAngleInPlane([1, 0, 0], [0, 0, 1], down)
          })
        )
      },
      // speed: () => {},
      // depth: () => {},
      north: () => {
        return downNorthObservable.pipe(
          map(([ down, north ]) => {
            return north
          })
        )
      },
      down: () => {
        return downNorthObservable.pipe(
          map(([ down, north ]) => {
            return down
          })
        )
      },
      // sensorToSubTransform: () => {},
      flatNorth: () => {
        return downNorthObservable.pipe(
          map(([ down, north ]) => {
            return geometry.projectOntoPlane(down, north)
          })
        )
      }
    })
    .whenIdleRun(
      Command()
        .named('Pose Calculation')
        .makeCancelable()
        .require('imu', 'pose')
        .action(system => {
          console.log('started pose calc')
          
          const angularVelocityObservable = system.imu.angularVelocity().pipe(map(geometry.vectorToArray)).pipe(
            map(([ x, y, z]) => ([-x / 2, -y / 2, -z / 2]))
          )
          const linearAccelerationObservable = system.imu.linearAcceleration().pipe(map(geometry.vectorToArray))
          const magneticFieldObservable = system.imu.magneticField().pipe(map(geometry.vectorToArray))
          
          // let count = 0
          const gyroRotationMatrixObservable = angularVelocityObservable.pipe(
            timeInterval(),
            map(({ value:angularVelocityVector, interval:deltaTime }) => {
              // console.log('toRotMatrix', count++)
              return toRotMatrix(angularVelocityVector, deltaTime / 1000.0)
            }),
            publish(),
            refCount()
          )
          
          let startedDown = false
          const lastDownSubject = new Subject()
          const filteredDownObservable = gyroRotationMatrixObservable.pipe(
            tap(() => {
              if(!startedDown){
                startedDown = true
                lastDownSubject.next([0, 0, 1]);
              }
            }),
            withLatestFrom(lastDownSubject),
            map(([ gyroRotationMatrix, lastDownVector ]) => {
              // rotate last down by gyro amount
              const newDownVector = math.multiply(gyroRotationMatrix, lastDownVector)
              return newDownVector
            }),
            withLatestFrom(linearAccelerationObservable),
            map(([ gyroDownVector, accelDownVector ]) => {
              // lerp between gyro prediction and accel data
              const filteredDown = geometry.normalizeVector(geometry.lerpVectors(geometry.normalizeVector(gyroDownVector), geometry.normalizeVector(accelDownVector), accelLerpAmount))
              lastDownSubject.next(filteredDown)
              return filteredDown
            }),
            publish(),
            refCount()
          )
          
          let startedNorth = false
          const lastNorthSubject = new Subject()
          const filteredNorthObservable = gyroRotationMatrixObservable.pipe(
            tap(() => {
              if(!startedNorth){
                startedNorth = true
                lastNorthSubject.next([1, 0, 0]);
              }
            }),
            withLatestFrom(lastNorthSubject),
            map(([ gyroRotationMatrix, lastNorthVector ]) => {
              // rotate last north by gyro amount
              const newNorthVector = math.multiply(gyroRotationMatrix, lastNorthVector)
              return newNorthVector
            }),
            withLatestFrom(magneticFieldObservable),
            map(([ gyroNorthVector, accelNorthVector ]) => {
              // lerp between gyro prediction and magnet data
              const filteredNorth = geometry.normalizeVector(geometry.lerpVectors(geometry.normalizeVector(gyroNorthVector), geometry.normalizeVector(accelNorthVector), magnetLerpAmount))
              lastNorthSubject.next(filteredNorth)
              return filteredNorth
            }),
            publish(),
            refCount()
          )
          
          downNorthObservable = zip(filteredDownObservable, filteredNorthObservable).pipe(
            map(args => {
              //console.log('zip:', args)
              if(isNaN(args[0][0])){
                throw Error('became NaN')
              }
              return args
            })  
          )
          
          return downNorthObservable // TODO add enable
        })
    )
}

function toRotMatrix(angularVelocityVector, deltaTime){
  const magnitude = math.norm(angularVelocityVector)
  const v = math.divide(angularVelocityVector, magnitude)
  const phi = magnitude * deltaTime
  const s = math.sin(phi)
  const c = math.cos(phi)
  return [
    [
      c + math.pow(v[x], 2) * (1 - c),
      v[x] * v[y] * (1 - c) - v[z] * s, 
      v[x] * v[z] * (1 - c) + v[y] * s
    ], [
      v[x] * v[y] * (1 - c) + v[z] * s, 
      c + math.pow(v[y], 2) * (1 - c), 
      v[y] * v[z] * (1 - c) - v[x] * s
    ], [
      v[x] * v[z] * (1 - c) - v[y] * s, 
      v[z] * v[y] * (1 - c) + v[x] * s, 
      c + math.pow(v[z], 2) * (1 - c)
    ]
  ]
}
