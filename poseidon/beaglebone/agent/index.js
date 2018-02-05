const comm = require('./extern/utils').comm

comm.create({
  address: 'localhost',
  port: 3001,
  separator: '|'
}).then(hub => {
  hub.on('echo/r', (hub, data) => console.log("Got echo:", data))
  hub.on('arduino', (hub, data) => console.log("Arduino:", data))

  // for(let i = 0; i < 10; i++)
  //   hub.emit('echo', [`This is message #${i}`])

  setInterval(hub.poll, 10)
}).catch(error => console.log(error))
