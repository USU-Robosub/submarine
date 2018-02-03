const net = require('net')

const createServer = (address, port) => {
  const clients = []
  const server = net.createServer()
  let connection = false;
  server.listen(port, address)
  server.on('error', error => {
    throw error
  })
  server.on('connection', client => {
    clients.push(client)
    if(connection){
      connection(client)
    }
  })
  return {
    connection: callback => connection = callback,
    destroy: callback => {
      clients.forEach(client => {
        client.end()
        client.destroy()
      })
      server.close(() => {
        server.unref()
        if(callback){
          callback()
        }
      })
    }
  }
}

module.exports = createServer
