const net = require('net')

const createClientPort = (address, port) => {
  const client = new net.Socket()
  const buffer = []
  const connectPromise = new Promise((resolve, reject) => {
    client.on('error', error => reject(error))
    client.on('connect', () => resolve({
      push: data => client.write(data),
      poll: () => buffer.splice(0, 1)[0],
      hasData: () => buffer.length > 0
    }))
    client.on('data', data => {
      buffer.push(data.toString())
    })
    client.connect(port, address)
  })
  return connectPromise
}

module.exports = createClientPort
