const hub = bridge => {
  const handlers = {};
  const hub = {
    on: (name, handler) => {
      if(!handlers[name]){
        handlers[name] = []
      }
      handlers[name].push(handler)
    },
    poll: () => {
      while(bridge.hasData()){
        const data = bridge.receive()
        const name = data[0]
        data.splice(0, 1)
        handlers[name].forEach(handler => handler(hub, data))
      }
    },
    emit: (name, data) => {
      bridge.send([name].concat(data))
    }
  }
  return hub
}

module.exports = hub
