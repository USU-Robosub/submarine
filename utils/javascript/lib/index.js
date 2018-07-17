const utils = {
  comm: require('../src/comm'),
  scheduler: {
    ...require('../src/scheduler'),
    tools: require('../src/scheduler/tools')
  }
}

module.exports = utils
