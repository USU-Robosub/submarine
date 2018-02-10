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

function setup() {
  createCanvas(600, 300);
}

const pos = {x:0, y:0}
const pos2 = {x:0, y:0}

function draw() {
  background(0);
  noStroke();

  fill(255);
  ellipse(150 + pos.x,  150 + pos.y, 72, 72);
  ellipse(450 + pos2.x,  150 + pos2.y, 72, 72);
}

var timer = Rx.Observable.timer(200, 10);

const axis = index => () => navigator.getGamepads()[gamepadIndex].axes[index]

const deadZone = (lower, upper) => x => x < lower ? x : x > upper ? x : 0;

const gamepad = timer.filter(() => gamepadIndex != -1)

const filteredAxis = index => gamepad.map(axis(index)).map(deadZone(-0.1, 0.1)).distinctUntilChanged().throttleTime(10)

filteredAxis(0).subscribe(axis => {
  console.log("axis", 0, axis)
  pos.x = axis * 100;
  socket.emit('steering', axis * 90 + 90)
})

filteredAxis(1).subscribe(axis => {
  console.log("axis", 1, axis)
  pos.y = axis * 100;
  socket.emit('throttle', -axis * 90 + 90)
})

filteredAxis(2).subscribe(axis => {
  console.log("axis", 2, axis)
  pos2.x = axis * 100;
})

filteredAxis(3).subscribe(axis => {
  console.log("axis", 3, axis)
  pos2.y = axis * 100;
  socket.emit('dive', axis * 90 + 90)
})
