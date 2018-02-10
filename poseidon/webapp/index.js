var socket = io();

// const gamepad = navigator.getGamepads()

let gamepadIndex = -1;

console.log('app started')
window.addEventListener("gamepadconnected", function(e) {
  console.log("Gamepad connected at index %d: %s. %d buttons, %d axes.",
    e.gamepad.index, e.gamepad.id,
    e.gamepad.buttons.length, e.gamepad.axes.length);
  gamepadIndex = e.gamepad.index
});

var timer = Rx.Observable.timer(200, 10);

const axis = index => () => navigator.getGamepads()[gamepadIndex].axes[index]

const deadZone = (lower, upper) => x => x < lower ? x : x > upper ? x : 0;

const gamepad = timer.filter(() => gamepadIndex != -1)

const filteredAxis = index => gamepad.map(axis(index)).map(deadZone(-0.1, 0.1)).distinctUntilChanged().throttleTime(10)

filteredAxis(0).subscribe(axis => {
  console.log("axis", 0, axis)
  socket.emit('steering', axis * 90 + 90)
})

filteredAxis(1).subscribe(axis => {
  console.log("axis", 1, axis)
  socket.emit('throttle', -axis * 90 + 90)
})

filteredAxis(2).subscribe(axis => {
  console.log("axis", 2, axis)
})

filteredAxis(3).subscribe(axis => {
  console.log("axis", 3, axis)
  socket.emit('dive', axis * 90 + 90)
})
