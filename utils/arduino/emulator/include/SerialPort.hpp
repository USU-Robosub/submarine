#ifndef SERIAL_PORT
#define SERIAL_PORT

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

class SerialPort{
public:
  SerialPort(std::string portName, speed_t speed);
  ~SerialPort();
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
