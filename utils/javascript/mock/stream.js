const createStream = data => {
  const stream = {
    poll: () => {
      const next = data[0]
      data.splice(0, 1)
      return next
    },
    hasData: () => {
      return data.length > 0
    },
    push: data => {
      stream.writeData.push(data)
    },
    writeData: []
  }
  return stream
}

module.exports = createStream
