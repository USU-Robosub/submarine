#ifndef COMM_SERIAL
#define COMM_SERIAL

/* Linux serial port headers */
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
/* --- */
#include <string>

namespace Comm{
  class Serial {
  public:
    Serial(std::string portName, unsigned int speed) : portName(portName), speed(speed) {}
    bool connect();
    void disconnect();
  private:
    bool configure();
    unsigned int speed;
    std::string portName;
    int fileDescriptor = 0;
  };
}

#endif
