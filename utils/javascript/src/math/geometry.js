const math = require("mathjs")

const vectorToArray = vector => [vector.x, vector.y, vector.z]

const normalizeVector = vector => math.divide(vector, math.norm(vector))

const lerpVectors = (start, target, amount) => 
  math.add(math.multiply(target, amount), math.multiply(start, 1 - amount))

const getMinimumAngle = (normal, v, w) => {
  const angle = math.acos(math.dot(math.divide(v, math.norm(v)), math.divide(w, math.norm(w))))
  const cross = math.cross(v, w)
  if(math.dot(normal, cross) < 0){
    return -angle
  }else{
    return angle
  }
}

const projectOntoPlane = (normal, vector) =>
  math.subtract(vector, math.multiply(math.divide(
    math.dot(normal, vector), math.pow(math.norm(normal), 2)
  ), normal))

const findAngleInPlane = (normal, v, w) => 
  getMinimumAngle(normal, projectOntoPlane(normal, v), projectOntoPlane(normal, w))
  
module.exports = {
  vectorToArray,
  normalizeVector,
  lerpVectors,
  getMinimumAngle,
  projectOntoPlane,
  findAngleInPlane
}