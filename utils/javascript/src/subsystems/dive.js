const { Subsystem } = require('../scheduler')

module.exports = (hub, handlerName="dive") => Subsystem()
  .named('dive')
  .raw({
    power: amount => {
      hub.emit(handlerName + '/power', [amount])
    },
  })
