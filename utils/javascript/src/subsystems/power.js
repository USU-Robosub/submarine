const { Subsystem, Command } = require('../scheduler')

module.exports = (hub, handlerName="power") => Subsystem()
    .named('power')
    .makeShared()
    .raw({
      status: enabled => {
        hub.emit(handlerName + '/enable', [enabled])
      },
      enable: () => {
        hub.emit(handlerName + '/enable', [1])
      },
      disable: () => {
        hub.emit(handlerName + '/enable', [0])
      }
  })
