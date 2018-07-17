#ifndef COMM_SERIAL_PORT
#define COMM_SERIAL_PORT

/* Linux serial port headers */
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/ioctl.h> //ioctl() call defenitions
/* --- */
#include <string>
#include <mutex>
#include <functional>
#include <Comm/Port.hpp>

namespace Comm{
  namespace Serial{
    class Port;
  }
}

class Comm::Serial::Port : public Comm::Port<unsigned char>{
public:
  Port(std::string portName, speed_t speed);
  ~Port();
  bool hasData();
  void push(const unsigned char* buffer, std::size_t length);
  std::size_t poll(unsigned char* buffer, std::size_t length);
  void restartArduino();
  static std::string portNameFromPath(std::string portPath);

private:
  void configure();

  int fileDescriptor = 0;
  std::string portName;
  unsigned int speed;
};

#endif
