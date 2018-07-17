#ifndef SERIAL_MOCK
#define SERIAL_MOCK

#define BUFFER_SIZE 1024

namespace Mock{
  class Serial{
  public:
    bool isStarted = false;

    void begin(int port) {
      this->isStarted = true;
    }

    int available(){
      return this->bufferLength - this->bufferReadIndex;
    }

    void write(unsigned char* buffer, long length){
      for(int i = 0; i < length; ++i){
        this->buffer[bufferWriteIndex++] = buffer[i];
      }
    }

    int16_t read() {
      return this->buffer[bufferReadIndex++];
    }

    explicit operator bool() const { return true; } // allows !Serial check to pass

    void $reset(){
      this->bufferReadIndex = 0;
      this->bufferWriteIndex = 0;
      this->bufferLength = 0;
      for(int i = 0; i < BUFFER_SIZE; ++i){
        this->buffer[i] = 0;
      }
    }

    void $loadBuffer(int32_t n, int32_t loc){
      this->buffer[loc * 4 + 0] = n >> 0 & 0xFF;
      this->buffer[loc * 4 + 1] = n >> 8 & 0xFF;
      this->buffer[loc * 4 + 2] = n >> 16 & 0xFF;
      this->buffer[loc * 4 + 3] = n >> 24 & 0xFF;
    }

    int32_t $nextInt(){
      return (this->$readInt()) |
             (this->$readInt() << 8) |
             (this->$readInt() << 16) |
             (this->$readInt() << 24);
    }

    int32_t $readInt(){
      return this->read();
    }

    void $bufferLength(int length){
      this->bufferLength = length * 4;
    }

    unsigned char buffer[BUFFER_SIZE] = {0};

  private:
    int bufferReadIndex = 0;
    int bufferWriteIndex = 0;
    int bufferLength = 0;
  };
}

extern Mock::Serial Serial;

#ifdef MOCK_INCLUDE_SOURCE

Mock::Serial Serial;

#endif
#endif
