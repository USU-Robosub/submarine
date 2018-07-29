let mode = "single";
let leftAxis = 0;
let rightAxis = 0;

// const gamepad = navigator.getGamepads()

let gamepadIndex = -1;
/* global socket, Rx, navigator  */
screen.log('app started')
window.addEventListener("gamepadconnected", function(e) {
  screen.log("Gamepad connected at index "+e.gamepad.index+": "+e.gamepad.id+". "+e.gamepad.buttons.length+" buttons, "+e.gamepad.axes.length+" axes.");
  gamepadIndex = 0
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
    if(!false){
      screen.log("tank/steering", axis)
    socket.emit('tank/steering', axis)
    }
  }
})

filteredAxis(1).subscribe(axis => {
  screen.log("axis", 1, axis)
  if(mode=="tank")
  {
    leftAxis = axis+1;
    screen.log("tank", leftAxis, rightAxis)
    socket.emit('tank/left', leftAxis)
    socket.emit('tank/right', rightAxis)
  }
  else { //Single Joystick
    if(!false){
    screen.log("tank/throttle", -axis)
      socket.emit('tank/throttle', -axis)
    }
  }
})

filteredAxis(2).subscribe(axis => {
  screen.log("axis", 2, axis)
  if(mode=="tank")
  {}
  else if(mode=="dual")
  {
    if(!false){
       screen.log("tank/steering", axis)
      socket.emit('tank/steering', axis)
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
    screen.log("tank", leftAxis, rightAxis)
    socket.emit('tank/left', leftAxis)
    socket.emit('tank/right', rightAxis)
  }
  else if(mode=="dual")
  {}
  else {//Single Joystick
    if(!false){
      screen.log("dive/power", axis)
      socket.emit('dive/power', axis)
    }
  }

})
