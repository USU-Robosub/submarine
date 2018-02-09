const comm = require('./extern/utils').comm
const path = require('path')
const app = require('express')()
const http = require('http').Server(app)
const io = require('socket.io')(http)

app.get('/', (req, res) => res.sendFile(path.resolve(__dirname + '/../../webapp/index.html')))
app.get('/socket.js', (req, res) => res.sendFile(path.resolve(__dirname + '/../../webapp/socketio/socket.io.min.js')))
app.get('/rx.js', (req, res) => res.sendFile(path.resolve(__dirname + '/../../webapp/rxjs/rx.min.js')))
app.get('/index.js', (req, res) => res.sendFile(path.resolve(__dirname + '/../../webapp/index.js')))

io.on('connection', function(socket){
  console.log('a user connected')
  socket.on('disconnect', function(){
    console.log('user disconnected')
  })
})

http.listen(3000, () => console.log('Web app listening on port 3000'))

// comm.create({
//   address: 'localhost',
//   port: 3001,
//   separator: '|'
// }).then(hub => {
//   hub.on('echo/r', (hub, data) => console.log("Got echo:", data))
//   hub.on('arduino', (hub, data) => console.log("Arduino:", data))
//
//   // for(let i = 0; i < 10; i++)
//   //   hub.emit('echo', [`This is message #${i}`])
//
//
//
//   setInterval(hub.poll, 10)
// }).catch(error => console.log(error))
