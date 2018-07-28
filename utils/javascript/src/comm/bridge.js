const MessageStates = {
  CHECK: 0,
  NAME: 1,
  LENGTH: 2,
  DATA: 3
}

const createBridge = stream => {
  let dataLeft = 0
  let currentMessage = {}
  let state = MessageStates.CHECK
  return {
    receive: () => {
      const messages = []
      let count = 0;
      while(stream.hasData() && count < 100){
        count++;
        switch(state){
          case MessageStates.CHECK:
            currentMessage.check = stream.poll()
            state = MessageStates.NAME
            break
          case MessageStates.NAME:
            currentMessage.name = stream.poll()
            state = MessageStates.LENGTH
            break
          case MessageStates.LENGTH:
            currentMessage.length = stream.poll()
            dataLeft = currentMessage.length
            if(dataLeft > 0){
              currentMessage.data = []
              state = MessageStates.DATA
            }else{
              messages.push([currentMessage.name])
              state = MessageStates.CHECK
            }
            break
          case MessageStates.DATA:
            currentMessage.data.push(stream.poll())
            dataLeft--
            if(dataLeft == 0){
              messages.push([currentMessage.name].concat(currentMessage.data))
              currentMessage = {}
              state = MessageStates.CHECK
            }
            break
        }
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
