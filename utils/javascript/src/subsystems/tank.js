const { Subsystem } = require('../scheduler')

module.exports = (hub, handlerName="tank") => Subsystem()
  .named('tank')
  .raw({
    throttle: amount => {
      hub.emit(handlerName + '/throttle', [amount])
    },
    steering: amount => {
      hub.emit(handlerName + '/steering', [amount])
    },
    left: amount => {
      hub.emit(handlerName + '/left', [amount])
    },
    right: amount => {
      hub.emit(handlerName + '/right', [amount])
    },
  })
