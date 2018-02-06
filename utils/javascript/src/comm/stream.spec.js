const createStream = require('./stream.js')
const createMockPort = require('../../mock').createPort

describe('stream can read from a port', () => {
  test('single string with "|" as splitter', () => {
    const stream = createStream(createMockPort(['hello world|']), '|')
    expect(stream.poll()).toBe('hello world')
  })

  test('single string with "_" as splitter', () => {
    const stream = createStream(createMockPort(['test|test_']), '_')
    expect(stream.poll()).toBe('test|test')
  })

  test('multiple strings with "|" as splitter', () => {
    const stream = createStream(createMockPort(['test 1|test 2|']), '|')
    expect(stream.poll()).toBe('test 1')
    expect(stream.poll()).toBe('test 2')
  })

  test('multiple port reads', () => {
    const stream = createStream(createMockPort(['test 1|test', ' 2|test 3|', 'test 4|']), '|')
    expect(stream.poll()).toBe('test 1')
    expect(stream.poll()).toBe('test 2')
    expect(stream.poll()).toBe('test 3')
    expect(stream.poll()).toBe('test 4')
  })
})

describe('stream reflects the has data of port', () => {
  test('shows no data', () => {
    const stream = createStream(createMockPort([]))
    expect(stream.hasData()).toBeFalsy()
  })

  test('shows data', () => {
    const stream = createStream(createMockPort(['test']), '|')
    expect(stream.hasData()).toBeTruthy()
  })

  test('shows data after reading', () => {
    const stream = createStream(createMockPort(['hello|world|']), '|')
    stream.poll()
    expect(stream.hasData()).toBeTruthy()
    stream.poll()
    expect(stream.hasData()).toBeFalsy()
  })
})

describe('stream can write to port', () => {
  test('"|" as separator', () => {
    const port = createMockPort()
    const stream = createStream(port, '|')
    stream.push('hello world')
    stream.push('test event')
    expect(port.writeData).toEqual('hello world|test event|')
  })

  test('"_" as separator', () => {
    const port = createMockPort()
    const stream = createStream(port, '_')
    stream.push('hello world')
    stream.push('test event')
    expect(port.writeData).toEqual('hello world_test event_')
  })
})
