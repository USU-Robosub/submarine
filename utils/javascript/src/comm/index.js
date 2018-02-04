const comm = {
  createHub: require('./hub'),
  tcp: {
    createPort: require('./tcp/port')
  },
  createStream: require('./stream'),
  createBridge: require('./bridge'),
  create: config => {
    return new Promise((resolve, reject) => {
      comm.tcp.createPort(config.address, config.port)
      .then(port => {
        resolve(comm.createHub(
          comm.createBridge(comm.createStream(port, config.separator))
        ))
      })
      .catch(reject)
    })
  }
}

module.exports = comm
