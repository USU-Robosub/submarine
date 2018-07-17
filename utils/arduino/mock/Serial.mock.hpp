#ifndef SERIAL_MOCK
#define SERIAL_MOCK

#define BUFFER_SIZE 1024

namespace Mock{
  class Serial{
  public:
    bool isStarted = false;

    void begin(int baud) {
      if(this->begin_itr != nullptr){
        this->begin_itr(baud);
        return;
      }
      this->isStarted = true;
    }

    int available(){
      if(this->available_itr != nullptr){
        return this->available_itr();
      }
      return this->bufferLength - this->bufferReadIndex;
    }

    void write(unsigned char* buffer, long length){
      if(this->write_itr != nullptr){
        this->write_itr(buffer, length);
        return;
      }
      for(int i = 0; i < length; ++i){
        this->buffer[bufferWriteIndex++] = buffer[i];
      }
    }

    int16_t read() {
      if(this->read_itr != nullptr){
        return this->read_itr();
      }
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

    void $intercept(int16_t (*read)(), void (*write)(unsigned char* buffer, long length), int (*available)(), void (*begin)(int port)){
      this->read_itr = read;
      this->write_itr = write;
      this->available_itr = available;
      this->begin_itr = begin;
    }

    unsigned char buffer[BUFFER_SIZE] = {0};

  private:
    int bufferReadIndex = 0;
    int bufferWriteIndex = 0;
    int bufferLength = 0;

    int16_t (*read_itr)() = nullptr;
    void (*write_itr)(unsigned char* buffer, long length) = nullptr;
    int (*available_itr)() = nullptr;
    void (*begin_itr)(int port) = nullptr;
  };
}

extern Mock::Serial Serial;

#ifdef MOCK_INCLUDE_SOURCE

Mock::Serial Serial;

#endif
#endif
