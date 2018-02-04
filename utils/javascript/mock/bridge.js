const createMockBridge = startData => {
  const bridge = {
    writeData: [],
    receive: () => {
      return startData
    },
    send: message => {
      bridge.writeData.push(message)
    }
  }
  return bridge
}

module.exports = createMockBridge;
