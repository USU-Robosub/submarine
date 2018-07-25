const { Subsystem } = require('../scheduler')
const { Observable } = require('rxjs')
const { map, tap } = require('rxjs/operators')

const attachToHubEvent = (hub, event) => Observable.create(observer => {
  const listener = hub.on(event, (hub, data) => {
    observer.next(data)
  })
  return () => {
    listener.remove()
  }
})

module.exports = (hub, handlerName="killSwitch") => Subsystem()
  .named('killSwitch')
  .makeShared()
  .raw({
    status: () => {
      return attachToHubEvent(hub, handlerName + '/status').pipe(
        map(data => (data[0] == 1 ? true : false))
      )
    }
  })
