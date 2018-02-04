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
      const messages = bridge.receive()
      messages.forEach(message => {
        const name = message[0]
        message.splice(0, 1)
        if(handlers[name]){
          handlers[name].forEach(handler => handler(hub, message))
        }else{
          console.log('Received event with no listener', name, message)
        }
      })
    },
    emit: (name, data) => {
      bridge.send([name].concat(data))
    }
  }
  return hub
}

module.exports = hub
