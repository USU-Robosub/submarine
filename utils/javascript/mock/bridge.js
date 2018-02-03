const createMockBridge = startData => {
  const bridge = {
    readData: startData,
    writeData: [],
    hasData: () => {
      return startData.length > 0
    },
    receive: () => {
      const message = bridge.readData[0]
      bridge.readData.splice(0, 1)
      return message
    },
    send: message => {
      bridge.writeData.push(message)
    }
  }
  return bridge
}

module.exports = createMockBridge;
