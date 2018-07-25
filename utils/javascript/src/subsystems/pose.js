const { Subsystem, Command } = require('../scheduler')
const { timeInterval, map, tap, withLatestFrom } = require('rxjs/operators')
const { zip, empty, Subject } = require('rxjs')
const math = require("mathjs")

const x = 0, y = 1, z = 2

const vectorToArray = vector => [vector.x, vector.y, vector.z]

const normalizeVector = vector => math.divide(vector, math.norm(vector, 3))

const lerpVectors = (start, target, amount) => 
  math.add(math.multiply(target, amount), math.multiply(start, 1 - amount))

function toRotMatrix(angularVelocityVector, deltaTime){
  const magnitude = math.norm(angularVelocityVector, 3)
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

const getMinimumAngle = (normal, v, w) => {
  const angle = math.acos(math.dot(v,w)/(math.norm(v)*math.norm(w)))
  const cross = math.cross(v, w)
  if(math.dot(normal, cross) < 0){
    return -angle
  }else{
    return angle
  }
}

const projectOntoPlane = (normal, vector) => 
  math.subtract(vector, math.multiply(math.divide(
    math.dot(normal, vector), math.pow(math.norm(normal, 3), 2)
  ), normal))

const findAngleInPlane = (normal, v, w) => 
  getMinimumAngle(normal, projectOntoPlane(normal, v), projectOntoPlane(normal, w))

module.exports = (hub, handlerName="pose") => {
  
  let enabled = false;
  let accelLerpAmount = 0.1;
  let magnetLerpAmount = 0.1;
  
  let downNorthObservable = empty()
  
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
            return findAngleInPlane(down, north, [1, 0, 0])
            //return findAngleInPlane([0, 0, 1], north, [1, 0, 0])
          })
        )
      },
      pitch: () => {
        return downNorthObservable.pipe(
          map(([ down, north ]) => {
            return findAngleInPlane([0, 1, 0], [0, 0, 1], down)
          })
        )
      },
      roll: () => {
        return downNorthObservable.pipe(
          map(([ down, north ]) => {
            return findAngleInPlane([1, 0, 0], [0, 0, 1], down)
          })
        )
      },
      speed: () => {},
      depth: () => {},
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
      sensorToSubTransform: () => {},
    })
    .whenIdleRun(
      Command()
        .named('Pose Calculation')
        .makeCancelable()
        .require('imu', 'pose')
        .action(system => {
          console.log('started pose calc')
          
          const angularVelocityObservable = system.imu.angularVelocity().pipe(map(vectorToArray)).pipe(
            map(([ x, y, z]) => ([-x / 10, -y / 10, -z / 10]))
          )
          const linearAccelerationObservable = system.imu.linearAcceleration().pipe(map(vectorToArray))
          const magneticFieldObservable = system.imu.megneticField().pipe(map(vectorToArray))
          
          const gyroRotationMatrixObservable = angularVelocityObservable.pipe(
            timeInterval(),
            map(({ value:angularVelocityVector, interval:deltaTime }) => {
              return toRotMatrix(angularVelocityVector, deltaTime / 1000.0)
            })
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
              const filteredDown = normalizeVector(lerpVectors(normalizeVector(gyroDownVector), normalizeVector(accelDownVector), accelLerpAmount))
              lastDownSubject.next(filteredDown)
              return filteredDown
            })
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
              const filteredNorth = normalizeVector(lerpVectors(normalizeVector(gyroNorthVector), normalizeVector(accelNorthVector), magnetLerpAmount))
              lastNorthSubject.next(filteredNorth)
              return filteredNorth
            })
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