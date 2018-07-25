const pid = (p, i, d) => {
  const errorIntegral = 0
  const lastError = 0
  return {
    correctFor: (error, deltaTime) => {
      errorIntegral = errorIntegral + error * deltaTime
      const derivative = (deltaTime == 0 ? 0 : (error - lastError) / deltaTime)
      lastError = error
      return p * error + i * errorIntegral + d * derivative
    }
  }
}

module.exports = pid