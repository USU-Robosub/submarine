const createPort = data => {
  const port = {
    poll: () => {
      const next = data[0]
      data.splice(0, 1)
      return next
    },
    hasData: () => {
      return data.length > 0
    },
    push: data => {
      port.writeData += data
    },
    writeData: ''
  }
  return port
}

module.exports = createPort
