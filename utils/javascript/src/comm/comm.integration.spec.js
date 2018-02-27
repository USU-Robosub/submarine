const comm = require('.')
const createMockServer = require('../../mock').createServer
let portfinder = require('portfinder')

let testPort = 3001
const testAddress = 'localhost'

let server = 0;

beforeEach(done => {
  portfinder.getPort(function (err, port) {
    testPort = port
    console.log('Running on port: ', testPort)
    server = createMockServer(testAddress, testPort)
    done()
  })
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
