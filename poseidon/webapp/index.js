var socket = io();

socket.on('disconnect', () => {
  screen.error('server disconnected')
});

socket.on('connect', () => {
  screen.error('server connected')
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
  screen.log('Lost control over dive subsystem')
})

let scrolllock = false;
let consolelog = "";
let lines = 0;
function logConsole(source, message) {
  let level = message.shift();
  let icon, color;
  switch(level) {
    case "0":
      icon = 'fa-info-circle';
      color = 'success';
      break;
    case "1":
      icon = 'fa-exclamation-circle';
      color = 'warn';
      break;
    default:
      icon = 'fa-times-circle';
      color = 'danger';
      break;
  }
  let newline =  "<span class='has-text-"+color+"'><i class='fas "+icon+"'></i> ["+source+"] </span> "+message.join(" ")+"\r\n";
  if(lines == 0) {
    document.getElementById("console").innerHTML = "";
  }
  if(lines>1000) {
    document.getElementById("console").innerHTML = document.getElementById("console").innerHTML.substring(document.getElementById("console").innerHTML.indexOf("\n") + 1)
    if(scrolllock) {
      let scrollHeight = document.getElementById("console").getElementsByTagName("span")[0].offsetHeight;
      let newScroll = document.getElementById("console").scrollTop - scrollHeight;
      if(newScroll < 0)
        newScroll = 0;
      document.getElementById("console").scrollTop = newScroll;
    }
  }
  else
  {
    lines++;
  }
  document.getElementById("console").innerHTML+=newline;
  if(!scrolllock)
    document.getElementById("console").scrollTop = document.getElementById("console").scrollHeight;
    
  console.log("%c["+source+"] "+message.join(" "), "color: "+(level=="0"?'darkgreen':level=="1"?'#a79100':'darkred'));
}



let screen = {
  log : (...args) => {
    logConsole("Local",["0",args.join(" ")]);
  },
  warn : (...args) => {
    logConsole("Local",["1",args.join(" ")]);
  },
  error : (...args) => {
    logConsole("Local",["2",args.join(" ")]);
  },
}

socket.on('echo/arduino', (message) => {
  logConsole("Arduino", message);
})

socket.on('echo/system', (message) => {
  logConsole("System", message);
})

socket.on('echo/agent', (message) => {
  logConsole("Agent", message);
})

socket.on('tank/lost', () => {
  screen.log('Lost control over tank subsystem')
})

/*global setSubmarineRotation */
socket.on('pose/all', ([ yaw, pitch, roll ]) => {
  setSubmarineRotation(yaw, 0, 0);
})

socket.on('pose/yaw', (angle) => {
  // console.log('angle', angle * (180 / Math.PI), angle)
  // setYawDirection(Math.cos(angle), 0, Math.sin(angle))
})

socket.on('pose/flatNorth', (args) => {
  //console.log(args)
  setYawDirection(args[0], args[1], args[2])
})

socket.on('pose/down', (args) => {
  setDownDirection(args[0], args[1], args[2])
  setDownPlane(args[0], args[1], args[2])
})


socket.on('pose/north', (args) => {
  // console.log(args)
  setMagnetFieldDirection(args[0], args[1], args[2])
})

socket.on('pose/flatForward', (args) => {
  //console.log(args)
  setForwardDirection(args[0], args[1], args[2])
})
// socket.on('imu/raw_large', data => {
//   setMagnetFieldDirection(data[3 * 3], data[3 * 3 + 1], data[3 * 3 + 2]);
// })

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

function setSteering(x){
  if(!killed) {
    socket.emit('steering', x)
    document.getElementById("steerstatus").innerHTML = x;
    document.getElementById("headingstatus").innerHTML = 'Stopped';
    curSteer = x;
  }
}

function setHeading(x){
  if(!killed) {
    socket.emit('heading', x)
    document.getElementById("steerstatus").innerHTML = 0;
    document.getElementById("headingstatus").innerHTML = x;
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
let leftSteer = function(){curSteer -= delta;setSteering(curSteer);};
let rightSteer = function(){curSteer += delta;setSteering(curSteer);};

let curHeading = 0;
let stopHeading = function(){curHeading = 0;stopSteer();};
let leftHeading = function(){curHeading -= delta;setHeading(curHeading);};
let rightHeading = function(){curHeading += delta;setHeading(curHeading);};

let curThrottle = 0;
let stopThrottle = function(){curThrottle = 0;setThrottle(curThrottle);};
let downThrottle = function(){curThrottle -= delta;setThrottle(curThrottle);};
let upThrottle = function(){curThrottle += delta;setThrottle(curThrottle);};

let mode = "single";
let leftAxis = 0;
let rightAxis = 0;

// const gamepad = navigator.getGamepads()

let gamepadIndex = -1;

screen.log('app started')
window.addEventListener("gamepadconnected", function(e) {
  screen.log("Gamepad connected at index %d: %s. %d buttons, %d axes.",
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
    screen.log("mode: single")
    mode="single"
  }
})

filteredButtons(5).subscribe(pressed => {
  if(pressed)
  {
    screen.log("mode: dual")
    mode="dual"
  }
})

filteredButtons(7).subscribe(pressed => {
  if(pressed)
  {
    screen.log("mode: tank")
    mode="tank"
  }
})

filteredButtons(4).subscribe(pressed => {
  if(pressed)
  {
    screen.log("mode: single")
    mode="single"
  }
})

filteredAxis(0).subscribe(axis => {
  screen.log("axis", 0, axis)
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
  screen.log("axis", 1, axis)
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
  screen.log("axis", 2, axis)
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
  screen.log("axis", 3, axis)
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
