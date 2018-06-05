#ifndef SERIAL_MOCK
#define SERIAL_MOCK

#include <types.mock.hpp>

#define BUFFER_SIZE 1024

namespace Mock{
  class Serial{
  public:
    bool isStarted = false;

    void begin(int buad);
    int available();
    void write(unsigned char* buffer, long length);
    int16_t read();
    explicit operator bool() const;
    void println(const char* message);
    void println();
    void print(const char* message);
    void print(const char message);
    void $reset();
    void $loadReadBuffer(int32_t n, int32_t loc);
    int32_t $nextInt();
    int32_t $readInt();
    int16_t $read();
    void $bufferLength(int length);
    void $print(const char* message);
    void $clearWriteBuffer();

    unsigned char readBuffer[BUFFER_SIZE] = {0};
    unsigned char writeBuffer[BUFFER_SIZE] = {0};

  private:
    int readBufferIndex = 0;
    int writeBufferIndex = 0;
    int readBufferLength = 0;

    int $readBufferIndex = 0;
    int $writeBufferIndex = 0;
  };
}

extern Mock::Serial Serial;

#ifdef MOCK_INCLUDE_SOURCE

void Mock::Serial::begin(int buad) {
  this->isStarted = true;
}

int Mock::Serial::available(){
  return this->readBufferLength - this->readBufferIndex;
}

void Mock::Serial::write(unsigned char* buffer, long length){
  for(int i = 0; i < length; ++i){
    this->writeBuffer[writeBufferIndex++] = buffer[i];
  }
}

int16_t Mock::Serial::read() {
  return this->readBuffer[readBufferIndex++];
}

Mock::Serial::operator bool() const { return true; } // allows !Serial check to pass

void Mock::Serial::println(const char* message){
  bool null = false;
  int index = 0;
  while(!null){
    char next = message[index++];
    if(next == 0){
      this->writeBuffer[this->writeBufferIndex++] = '\n';
      null = true;
    }else{
      this->writeBuffer[this->writeBufferIndex++] = next;
    }
  }
}

void Mock::Serial::println(){
  this->writeBuffer[this->writeBufferIndex++] = '\n';
}

void Mock::Serial::print(const char* message){
  bool null = false;
  int index = 0;
  while(!null){
    char next = message[index++];
    if(next == 0){
      null = true;
    }else{
      this->writeBuffer[this->writeBufferIndex++] = next;
    }
  }
}

void Mock::Serial::print(const char message){
  this->writeBuffer[this->writeBufferIndex++] = message;
}

void Mock::Serial::$reset(){
  this->isStarted = false;
  this->readBufferIndex = 0;
  this->writeBufferIndex = 0;
  this->readBufferLength = 0;
  this->$readBufferIndex = 0;
  this->$writeBufferIndex = 0;
  for(int i = 0; i < BUFFER_SIZE; ++i){
    this->readBuffer[i] = 0;
    this->writeBuffer[i] = 0;
  }
}

void Mock::Serial::$loadReadBuffer(int32_t n, int32_t loc){
  this->readBuffer[loc * 4 + 0] = n >> 0 & 0xFF;
  this->readBuffer[loc * 4 + 1] = n >> 8 & 0xFF;
  this->readBuffer[loc * 4 + 2] = n >> 16 & 0xFF;
  this->readBuffer[loc * 4 + 3] = n >> 24 & 0xFF;
}

int32_t Mock::Serial::$nextInt(){
  return (this->$readInt()) |
         (this->$readInt() << 8) |
         (this->$readInt() << 16) |
         (this->$readInt() << 24);
}

int32_t Mock::Serial::$readInt(){
  return this->$read();
}

int16_t Mock::Serial::$read() {
  return this->writeBuffer[$writeBufferIndex++];
}

void Mock::Serial::$bufferLength(int length){
  this->readBufferLength = length * 4;
}

void Mock::Serial::$print(const char* message){
  bool null = false;
  int index = 0;
  while(!null){
    char next = message[index++];
    if(next == 0){
      null = true;
    }else{
      this->readBuffer[this->$readBufferIndex++] = next;
      this->readBufferLength++;
    }
  }
}

void Mock::Serial::$clearWriteBuffer(){
  this->writeBufferIndex = 0;
  this->$writeBufferIndex = 0;
  for(int i = 0; i < BUFFER_SIZE; ++i){
    this->writeBuffer[i] = 0;
  }
}

Mock::Serial Serial;

#endif
#endif
