#ifndef COMM_SERIAL
#define COMM_SERIAL

/* Linux serial port headers */
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
/* --- */
#include <string>
#include <mutex>
#include <queue>
#include "Comm/Status.h"
#include "Comm/Stream.h"

namespace Comm{
  struct Data{
    char* data;
    unsigned short length;
  };

  class Serial : public Stream{
  public:
    Serial(std::string portName, unsigned int speed) : portName(portName), speed(speed) {}
    void connect();
    Status status();
    void disconnect();
    bool canRead();
    void send(char* data, unsigned int length);
    void receive(char* data, unsigned int length);
  private:
    void configure();
    void triggerError();
    Status _status = Status::Disconnected;
    int fileDescriptor = 0;
    std::string portName;
    unsigned int speed;
  };
}

#endif
