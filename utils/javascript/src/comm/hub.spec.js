const createHub = require('./hub.js')
const createMockBridge = require('../../mock').createBridge

test('hub can handle single event with one handler', done => {
  const hub = createHub(createMockBridge([
    ['event', 'item 1', 'item 2', 'item 3']
  ]))
  hub.on('event', (passedHub, data) => {
    expect(passedHub).toBe(hub)
    expect(data).toEqual(['item 1', 'item 2', 'item 3'])
    done();
  })
  hub.poll()
})

test('hub can handle single event with multiple handlers', () => {
  const hub = createHub(createMockBridge([
    ['event', 'item 1', 'item 2', 'item 3']
  ]))
  let count = 0
  hub.on('event', (passedHub, data) => count++)
  hub.on('event', (passedHub, data) => count++)
  hub.poll()
  expect(count).toBe(2)
})

test('hub can handle multiple events with multiple handlers', () => {
  const hub = createHub(createMockBridge([
    ['3', 'item 1', 'item 2', 'item 3'],
    ['2', 'item 1', 'item 2'],
    ['1', 'item 1']
  ]))
  let count = 0
  hub.on('1', (passedHub, data) => {
    expect(data).toEqual(['item 1'])
    count++
  })
  hub.on('2', (passedHub, data) => {
    expect(data).toEqual(['item 1', 'item 2'])
    count++
  })
  hub.on('3', (passedHub, data) => {
    expect(data).toEqual(['item 1', 'item 2', 'item 3'])
    count++
  })
  hub.poll()
  expect(count).toBe(3)
})

test('hub can send an event', () => {
  const bridge = createMockBridge()
  const hub = createHub(bridge)
  hub.emit('event', ['1', '2', '3', '4'])
  expect(bridge.writeData).toEqual([['event', '1', '2', '3', '4']])
})
