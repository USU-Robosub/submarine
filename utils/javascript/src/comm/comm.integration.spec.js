const comm = require('.')
const createMockServer = require('../../mock').createServer

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

test('complete comm integration check', done => {
  server.connection(client => {
    client.write('0|test|3|hello|world|!|')
  })

  comm.create({address: testAddress, port: testPort, separator: '|' })
  .then(hub => {
    const runHub = () => {
      hub.on('test', (hub, data) => {
        expect(data).toEqual(['hello', 'world', '!'])
        done()
      })
      hub.poll()
    }
    setTimeout(runHub, 100)
  })
})
