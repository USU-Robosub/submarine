const comm = require('./extern/utils').comm

comm.create({
  address: 'localhost',
  port: 3001,
  separator: '|'
}).then(hub => {
  let shouldExit = false;
  const updateHub = () => {
    hub.poll()
    if(!shouldExit){
      setTimeout(updateHub, 100)
    }
  }

  hub.on('echo/r', (hub, data) => {
    console.log("got echo", data)
  })

  hub.emit('echo', ['this is a test'])

  setTimeout(updateHub, 1000)
}).catch(error => console.log(error))
