#ifndef COMM_SERIAL
#define COMM_SERIAL

/* Linux serial port headers */
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
/* --- */
#include <string>
#include <mutex>
#include <queue>

namespace Comm{
  enum Status{
    Idle, Connecting, Ready, Error
  };

  struct Data{
    char* data;
    unsigned short length;
  };

  class Serial {
  public:
    Serial(std::string portName, unsigned int speed) : portName(portName), speed(speed) {}
    void connect();
    Status status();
    void disconnect();
    void send(char* data, unsigned short length);
    Data read();
  private:
    void updateBuffer();
    void triggerError();
    void configure();
    std::queue<Comm::Data> dataBuffer;
    char* rawBuffer;
    unsigned int rawBufferLength = 0;
    Status _status = Status::Idle;
    std::mutex threadLock;
    unsigned int speed;
    std::string portName;
    int fileDescriptor = 0;
  };
}

#endif
