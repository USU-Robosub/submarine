var socket = io();

const gamepad = navigator.getGamepads()

var timer = Rx.Observable.timer(200, 10);

function getValue(x){ return x.value; }
function sameValue(a, b){ return Math.abs(a - b) < 0.01; }
function checkAxis(name, x){ return x.axis == name; }
function deadZone(x) { return Math.abs(x) < 0.2 ? 0 : x; }
function modmod(x, n) { return x - Math.floor(x/n) * n; }
function getAngle(a, b, n) { return modmod(a - b + n, n * 2) - n }

var throttle = timer.map(function() {
  return gamepad.gamepads[0].state["RIGHT_BOTTOM_SHOULDER"];
}).map(deadZone).throttle(100).distinctUntilChanged(null, sameValue);

var steering = timer.map(function() {
  return gamepad.gamepads[0].state["LEFT_STICK_X"];
}).map(deadZone).throttle(100).distinctUntilChanged(null, sameValue);

var depthThrottle = 0;
var depth = timer.map(function() {
  return gamepad.gamepads[0].state["RIGHT_STICK_Y"];
}).map(deadZone).throttle(100).distinctUntilChanged(null, sameValue);

throttle.subscribe(function(x) {
  socket.emit('throttle', x)
});

steering.subscribe(function(x) {
  socket.emit('depth', x)
});

depth.subscribe(function(x) {
  socket.emit('depth', x)
});
