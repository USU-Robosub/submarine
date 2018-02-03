const createBridge = stream => {
  return {
    receive: () => {
      const messages = []
      while(stream.hasData()){
        const empty = stream.poll() // must be 0
        const name = stream.poll()
        const length = parseInt(stream.poll())
        const data = []
        for(let i = 0; i < length; i++){
          data.push(stream.poll())
        }
        messages.push([name].concat(data))
      }
      return messages
    },
    send: data => {
      stream.push('0')
      stream.push(data[0])
      stream.push((data.length - 1).toString())
      for(let i = 1; i < data.length; i++){
        stream.push(data[i])
      }
    }
  }
}

module.exports = createBridge
