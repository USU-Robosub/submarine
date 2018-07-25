var socket = io();

socket.on('disconnect', () => {
  console.error('server disconnected')
});

socket.on('connect', () => {
  console.error('server connected')
});

var killed = false;

socket.on("killswitch", ( hub, message )=>{
  killed = (message.length < 1 || message[0]!=="1");
  if(killed) {
    document.getElementById("killswitch").style.display = "block";
  } else {
    document.getElementById("killswitch").style.display = "none";
  }
});

socket.on('dive/lost', () => {
  console.log('Lost control over dive subsystem')
})

socket.on('tank/lost', () => {
  console.log('Lost control over tank subsystem')
})

/*global setSubmarineRotation */
socket.on('pose/all', ([ yaw, pitch, roll ]) => {
  setSubmarineRotation(yaw, 0, 0);
})

function setTank(left, right){
  if(!killed) {
    let throttle = 2 - (left + right)/2;
    let steering = (right-left)/2+1;
    socket.emit('throttle', throttle*90);
    socket.emit('steering', steering*90);
  }
}
let thrustTimeout = false;
function setThrottle(x){
  if(!killed) {
    socket.emit('throttle', x)
    document.getElementById("thruststatus").innerHTML = x;
    curThrottle = x;
    if(thrustTimeout!==false)
      clearTimeout(thrustTimeout);
    thrustTimeout = setTimeout(()=>setThrottle(curThrottle),250)
  }
}

let steerTimeout = false;
function setSteering(x){
  if(!killed) {
    socket.emit('steering', x)
    document.getElementById("steerstatus").innerHTML = x;
    curSteer = x;
    if(steerTimeout!==false)
      clearTimeout(steerTimeout);
    steerTimeout = setTimeout(()=>setSteering(curSteer),250)
  }
}

function setHeading(x){
  if(!killed) {
    socket.emit('heading', x)
    document.getElementById("steerstatus").innerHTML = x;
  }
}

let curLeft = 0;
let leftTimeout = false;
function setLeft(x){
  if(!killed) {
    socket.emit('left', x)
    curLeft = x;
    if(leftTimeout!==false)
      clearTimeout(leftTimeout);
    leftTimeout = setTimeout(()=>setLeft(curLeft),250)
  }
}

let curRight = 0;
let rightTimeout = false;
function setRight(x){
  if(!killed) {
    socket.emit('right', x)
    curRight = x;
    if(rightTimeout!==false)
      clearTimeout(rightTimeout);
    rightTimeout = setTimeout(()=>setRight(curRight),250)
  }
}

let diveTimeout = false;

function setDive(x){
  if(!killed) {
    socket.emit('dive', x)
    document.getElementById("divestatus").innerHTML = x;
    curDive = x;
    if(diveTimeout!==false)
      clearTimeout(diveTimeout);
    diveTimeout = setTimeout(()=>setDive(curDive),250)
  }
}

let diveSteerTimeout = false;
let curDiveSteer = 0;
function setDiveSteer(x){
  if(!killed) {
    socket.emit('dive/steer', x)
    document.getElementById("divesteerstatus").innerHTML = x;
    curDiveSteer = x;
    if(diveSteerTimeout!==false)
      clearTimeout(diveSteerTimeout);
    diveSteerTimeout = setTimeout(()=>setDiveSteer(curDiveSteer),250)
  }
}

const delta = 0.05;

let curDive = 0;
let stopDive = function(){curDive = 0;setDive(curDive);};
let downDive = function(){curDive -= delta;setDive(curDive);};
let upDive = function(){curDive += delta;setDive(curDive);};

let stopDiveSteer = function(){curDiveSteer = 0;setDiveSteer(curDiveSteer);};
let leftDiveSteer = function(){curDiveSteer += delta;setDiveSteer(curDiveSteer);};
let rightDiveSteer = function(){curDiveSteer -= delta;setDiveSteer(curDiveSteer);};

let curSteer = 0;
let stopSteer = function(){curSteer = 0;setSteering(curSteer);};
let leftSteer = function(){curSteer -= delta;setHeading(curSteer);};
let rightSteer = function(){curSteer += delta;setHeading(curSteer);};

let curThrottle = 0;
let stopThrottle = function(){curThrottle = 0;setThrottle(curThrottle);};
let downThrottle = function(){curThrottle -= delta;setThrottle(curThrottle);};
let upThrottle = function(){curThrottle += delta;setThrottle(curThrottle);};

let mode = "single";
let leftAxis = 0;
let rightAxis = 0;

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

const button = index => () => navigator.getGamepads()[gamepadIndex].buttons[index].pressed

const deadZone = (lower, upper) => x => x < lower ? x : x > upper ? x : 0;

const gamepad = timer.filter(() => gamepadIndex != -1)

const filteredAxis = index => gamepad.map(axis(index)).map(deadZone(-0.1, 0.1)).distinctUntilChanged().throttleTime(10)

const filteredButtons = index => gamepad.map(button(index)).distinctUntilChanged()

filteredButtons(6).subscribe(pressed => {
  if(pressed)
  {
    console.log("mode: single")
    mode="single"
  }
})

filteredButtons(5).subscribe(pressed => {
  if(pressed)
  {
    console.log("mode: dual")
    mode="dual"
  }
})

filteredButtons(7).subscribe(pressed => {
  if(pressed)
  {
    console.log("mode: tank")
    mode="tank"
  }
})

filteredButtons(4).subscribe(pressed => {
  if(pressed)
  {
    console.log("mode: single")
    mode="single"
  }
})

filteredAxis(0).subscribe(axis => {
  console.log("axis", 0, axis)
  if(mode=="tank")
  {}
  else if(mode=="dual")
  {}
  else {//Single Joystick
    if(!killed){
    socket.emit('steering', axis * 90 + 90)
    }
  }
})

filteredAxis(1).subscribe(axis => {
  console.log("axis", 1, axis)
  if(mode=="tank")
  {
    leftAxis = axis+1;
    setTank(leftAxis, rightAxis)
  }
  else { //Single Joystick
    if(!killed){
      socket.emit('throttle', -axis * 90 + 90)
    }
  }
})

filteredAxis(2).subscribe(axis => {
  console.log("axis", 2, axis)
  if(mode=="tank")
  {}
  else if(mode=="dual")
  {
    if(!killed){
      socket.emit('steering', axis * 90 + 90)
    }
  }
  else//Single Joystick
  {}
})

filteredAxis(3).subscribe(axis => {
  console.log("axis", 3, axis)
  if(mode=="tank")
  {
    rightAxis = axis+1
    setTank(leftAxis,rightAxis)
  }
  else if(mode=="dual")
  {}
  else {//Single Joystick
    if(!killed){
      socket.emit('dive', axis * 90 + 90)
    }
  }

})
