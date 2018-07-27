const pid = (p, i, d) => {
  let errorIntegral = 0
  let lastError = 0
  return {
    correctFor: (error, deltaTime) => {
      // console.log("Correct for", error, deltaTime)
      errorIntegral = errorIntegral + error * deltaTime
      const derivative = (deltaTime == 0 ? 0 : (error - lastError) / deltaTime)
      lastError = error
      return p * error + i * errorIntegral + d * derivative
    }
  }
}

module.exports = pid