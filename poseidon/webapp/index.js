var socket = io();

socket.on('disconnect', () => {
  screen.error('server disconnected')
});

socket.on('connect', () => {
  screen.good('server connected')
});

let disabled = false;
socket.on("killswitch/on", ()=>{
  screen.good("submarine enabled")
  disabled = false;
  }
);
socket.on("killswitch/off", ()=>{
  screen.warn("submarine disabled")
  disabled = true;
  }
);

socket.on('dive/lost', () => {
  screen.log('Lost control over dive subsystem')
  Array.from(document.getElementsByTagName('button')).forEach(function(elm){elm.disabled = true;});
})

socket.on('tank/lost', () => {
  screen.log('Lost control over tank subsystem')
  Array.from(document.getElementsByTagName('button')).forEach(function(elm){elm.disabled = true;});
})


function emit(command, ...message) {
  socket.emit(command, ...message)
  console.log(...message)
  screen.log(command, ...message)
}

let scrolllock = false;
let lines = 0;
function logConsole(source, message) {
  let level = message.shift();
  if(level <= 2) {
    let icon, color;
    switch(level) {
      case "-1":
        icon = 'fa-check-circle ';
        color = 'success';
        break;
      case "0":
        icon = 'fa-info';
        color = 'info';
        break;
      case "1":
        icon = 'fa-exclamation-triangle';
        color = 'warning';
        break;
      case "2":
        icon = 'fa-times-circle';
        color = 'danger';
        break;
      default:
        icon = 'fa-info';
        color = 'info';
        break;
    }
    let time  = new Date();
    let timeString = time.toLocaleTimeString();
  
    let newline =  "<span class='log-line'><span class='icon is-small has-text-"+color+"' title='"+timeString+"'><i class='fas "+icon+"'></i></span><span class='source has-text-"+color+"'>["+source+"]</span><span class='log-content'> "+message.join(" ")+"</span></span>\r\n";
    let terminal = document.getElementById("console")
    if(lines == 0) {
       while (terminal.hasChildNodes()) {
            terminal.removeChild(terminal.lastChild);
        }
    }
    if(lines>1000) {
      terminal.removeChild(terminal.getElementsByTagName("span")[0]);
      if(scrolllock) {
        let scrollHeight = terminal.getElementsByTagName("span")[0].offsetHeight;
        let newScroll = terminal.scrollTop - scrollHeight;
        if(newScroll < 0)
          newScroll = 0;
        terminal.scrollTop = newScroll;
      }
      terminal.insertAdjacentHTML('beforeend', newline);
    }
    else
    {
      lines++;
    }
    terminal.innerHTML+=newline;
    if(!scrolllock)
      terminal.scrollTop = terminal.scrollHeight;
  }
  console.log("%c["+source+"] "+message.join(" "), "color: "+(level=="0"?'darkgreen':level=="1"?'#a79100':(level=="2"?'darkred':'darkblue')));
}



let screen = {
  good : (...args) => {
    logConsole("Local",["-1",args.join(" ")]);
  },
  log : (...args) => {
    logConsole("Local",["0",args.join(" ")]);
  },
  warn : (...args) => {
    logConsole("Local",["1",args.join(" ")]);
  },
  error : (...args) => {
    logConsole("Local",["2",args.join(" ")]);
  },
  verbose : (...args) => {
    logConsole("Local",["3",args.join(" ")]);
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

/*global setSubmarineRotation */
socket.on('pose/all', ([ yaw, pitch, roll ]) => {
  setSubmarineRotation(yaw, 0, 0);
})

socket.on('pose/yaw', (angle) => {
  //console.log('angle', angle * (180 / Math.PI))
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

function setThrottle(x){
  if(!disabled) {
    socket.emit('throttle', x)
    screen.log('throttle', x);
  }
}

function setSteering(x){
  if(!disabled) {
    socket.emit('steering', x)
  }
}

function setHeading(x){
  if(!disabled) {
    socket.emit('heading', x)
  }
}

function setLeft(x){
  if(!disabled) {
    socket.emit('left', x)
  }
}

function setRight(x){
  if(!disabled) {
    socket.emit('right', x)
  }
}

function setDive(x){
  if(!disabled) {
    socket.emit('dive', x)
    screen.log('dive',x)
  }
}

function setDepth(x){
  if(!disabled) {
    socket.emit('depth', x)
    screen.log('depth',x)
  }
}

function setDiveTrim(x){
  if(!disabled) {
    socket.emit('dive/steer', x)
    screen.log('dive/steer',x)
  }
}

function setPitch(x){
  if(!disabled) {
    socket.emit('dive/steer', x)
    screen.log('dive/steer',x)
  }
}
