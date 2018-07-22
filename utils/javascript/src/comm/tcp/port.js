const net = require('net')

const createClientPort = (address, port) => {
  const client = new net.Socket()
  const buffer = []
  const connectPromise = new Promise((resolve, reject) => {
    let wasConnected = false;
    client.on('error', error => reject(error))
    client.on('connect', () => {
      wasConnected = true
      resolve({
        push: data => client.write(data),
        poll: () => buffer.splice(0, 1)[0],
        hasData: () => buffer.length > 0
      })
    })
    client.on('close', (...args) => {
      if(wasConnected){
        console.log('Port closed. To stop undefined behavior app will now force close.')
        process.kill(process.pid, "SIGINT") // send kill signal to process
      }
    })
    client.on('data', data => {
      buffer.push(data.toString())
    })
    client.connect(port, address)
  })
  return connectPromise
}

module.exports = createClientPort
