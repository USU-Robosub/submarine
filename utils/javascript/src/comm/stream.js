const createStream = (port, separator) => {
  let rawBuffer = '', tokenBuffer = []
  return {
    poll: () => {
      if(tokenBuffer.length == 0){
        rawBuffer += port.poll()
        let current = ''
        for(let i = 0; i < rawBuffer.length; i++){
          if(rawBuffer[i] == separator){
            tokenBuffer.push(current)
            current = ''
          }else{
            current += rawBuffer[i]
          }
        }
        rawBuffer = current
      }
      const next = tokenBuffer[0]
      tokenBuffer.splice(0, 1)
      return next
    },
    hasData: () => {
      return tokenBuffer.length > 0 || port.hasData()
    },
    push: data => {
      port.push(data + separator)
    }
  }
}

module.exports = createStream
