const comm = require('./extern/utils').comm
const path = require('path')
const app = require('express')()
const http = require('http').Server(app)
const io = require('socket.io')(http)

app.get('/', (req, res) => res.sendFile(path.resolve(__dirname + '/../webapp/index.html')))
app.get('/socket.js', (req, res) => res.sendFile(path.resolve(__dirname + '/../webapp/socketio/socket.io.min.js')))
app.get('/rx.js', (req, res) => res.sendFile(path.resolve(__dirname + '/../webapp/rxjs/rx.min.js')))
app.get('/index.js', (req, res) => res.sendFile(path.resolve(__dirname + '/../webapp/index.js')))

 comm.create({
   address: 'localhost',
   port: 3001,
   separator: '|'
 }).then(hub => {

  io.on('connection', function(socket){
    console.log('a user connected')
    socket.on('disconnect', function(){
      console.log('user disconnected')
    })
    socket.on('throttle', function(x){
      console.log('throttle', x)
      hub.emit('throttle', x)
    })
    socket.on('steering', function(x){
      console.log('steering', x)
      hub.emit('steering', x)
    })
    socket.on('dive', function(x){
      console.log('dive', x)
      hub.emit('dive', x)
    })
  })

   setInterval(hub.poll, 10)
 }).catch(error => console.log(error))

http.listen(3000, () => console.log('Web app listening on port 3000'))
