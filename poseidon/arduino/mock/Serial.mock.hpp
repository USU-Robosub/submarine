#ifndef SERIAL_MOCK
#define SERIAL_MOCK

#include <cstdint>

#define BUFFER_SIZE 500

namespace Mock{
  class Serial;
}

class Mock::Serial{
public:
  void begin(int port) {}
  explicit operator bool() const { return true; }
  int16_t read() {
    return this->buffer[bufferReadIndex++];
  }
  void write(unsigned char* buffer, long length){
    for(int i = 0; i < length; ++i){
      this->buffer[bufferWriteIndex++] = buffer[i];
    }
  }
  int available(){
    return this->_bufferLength - this->bufferReadIndex;
  }
  void loadBuffer(int32_t n, int32_t loc){
    this->buffer[loc * 4 + 0] = n >> 0 & 0xFF;
    this->buffer[loc * 4 + 1] = n >> 8 & 0xFF;
    this->buffer[loc * 4 + 2] = n >> 16 & 0xFF;
    this->buffer[loc * 4 + 3] = n >> 24 & 0xFF;
  }
  void bufferLength(int length){
    this->_bufferLength = length * 4;
  }
  int32_t nextInt(){
    return (this->readInt()) |
           (this->readInt() << 8) |
           (this->readInt() << 16) |
           (this->readInt() << 24);
  }
  int32_t readInt(){
    return this->read();
  }
  void reset(){
    this->bufferReadIndex = 0;
    this->bufferWriteIndex = 0;
    this->_bufferLength = 0;
    for(int i = 0; i < BUFFER_SIZE; ++i){
      this->buffer[i] = 0;
    }
  }

  unsigned char buffer[BUFFER_SIZE] = {0};

private:
  int bufferReadIndex = 0;
  int bufferWriteIndex = 0;
  int _bufferLength = 0;
};

//static Mock::Serial Serial;
extern Mock::Serial Serial;

#endif
