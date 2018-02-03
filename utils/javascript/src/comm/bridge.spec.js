const createBridge = require('./bridge')
const createMockStream = require('../../mock').createStream

describe('bridge can construct a message from stream', () => {
  test('single message', () => {
    const bridge = createBridge(createMockStream(
      ['0', 'name', '3', 'hello', 'world', '!']
    ))
    expect(bridge.receive()).toEqual([['name', 'hello', 'world', '!']])
  })

  test('multiple messages', () => {
    const bridge = createBridge(createMockStream(
      ['0', 'event', '2', '999', '000', '0', 'test', '0']
    ))
    expect(bridge.receive()).toEqual([['event', '999', '000'], ['test']])
  })
})

test('bridge can send messages', () => {
  const stream = createMockStream()
  const bridge = createBridge(stream)
  bridge.send(['name', 'item 1', 'item 2'])
  bridge.send(['test', '1', '2', '3'])
  expect(stream.writeData).toEqual([
    '0', 'name', '2', 'item 1', 'item 2',
    '0', 'test', '3', '1', '2', '3'
  ])
})
