const { Observable } = require('rxjs')
const { publish, refCount } = require('rxjs/operators')

const listenToHub = (hub, event) => Observable.create(observer => {
  const listener = hub.on(event, (hub, data) => {
    observer.next(data)
  })
  return () => {
    listener.remove()
  }
}).pipe(
  publish(), // force only one hub listener even with multiple subscribers
  refCount()
)

module.exports = {
  listenToHub 
}