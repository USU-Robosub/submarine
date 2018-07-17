const comm = require('./utils').comm
const path = require('path')
const express = require('express')
const app = express();
const http = require('http').Server(app)
const io = require('socket.io')(http)

app.get('/', (req, res) => res.sendFile(path.resolve(__dirname + '/../webapp/index.html')))
app.get('/rsapp/socket.js', (req, res) => res.sendFile(path.resolve(__dirname + '/../../extern/socketio/socket.io.min.js')))
app.get('/rsapp/rx.js', (req, res) => res.sendFile(path.resolve(__dirname + '/../../extern/rxjs/rx.min.js')))
app.get('/rsapp/index.js', (req, res) => res.sendFile(path.resolve(__dirname + '/../webapp/index.js')))
app.use(express.static(path.resolve(__dirname + '/../webapp')))
function init(){
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
    hub.on('killswitch', function(message){
      console.log('kill switch', message)
      socket.emit(message)
    })
  })

   setInterval(hub.poll, 10)
 }).catch(error => {
   console.log(error)
   setTimeout(init, 500)
 })
}

init()

http.listen(1234, () => console.log('Web app listening on port 3000'))
