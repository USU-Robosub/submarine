const createPort = require('./port.js')
const createMockServer = require('../../../mock').createServer

const testPort = 3001
const testAddress = 'localhost'

let server = 0;

beforeEach(() => {
  server = createMockServer(testAddress, testPort)
})

afterEach(() => {
  server.destroy()
  server = 0
})

test('tcp port can connect to server', done => {
  createPort(testAddress, testPort).then(port => {
    expect(port).toBeDefined()
    done()
  })
})

test('tcp port fails on connect if no server', done => {
  server.destroy(() => {
    createPort(testAddress, testPort).then(() => {
      throw 'TCP port should not connect.'
    }).catch(() => {
      done()
    })
  })
})

test('tcp port can send data', done => {
  server.connection(client => {
    client.on('data', data => {
      expect(data.toString()).toBe('test test test')
      done()
    })
  })
  createPort(testAddress, testPort).then(port => {
    port.push('test test test')
  })
})

test('tcp port should have data', done => {
  server.connection(client => {
    client.write('some data')
  })
  createPort(testAddress, testPort).then(port => {
    const checkData = () => {
      expect(port.hasData()).toBeTruthy()
      done()
    }
    setTimeout(checkData, 10)
  })
})

test('tcp port can read data', done => {
  server.connection(client => {
    client.write('hello world')
  })
  createPort(testAddress, testPort).then(port => {
    const checkData = () => {
      expect(port.poll()).toBe('hello world')
      expect(port.hasData()).toBeFalsy()
      done()
    }
    setTimeout(checkData, 10)
  })
})
