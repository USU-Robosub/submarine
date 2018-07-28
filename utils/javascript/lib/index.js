const utils = {
  comm: require('../src/comm'),
  scheduler: {
    ...require('../src/scheduler'),
    tools: require('../src/scheduler/tools')
  },
  subsystems: {
    ...require('../src/subsystems')
  },
  commands: {
    ...require('../src/commands')
  },
  pid: {
    ...require('../src/PID/pid.js')
  },
  math: require('../src/math')
}

module.exports = utils
