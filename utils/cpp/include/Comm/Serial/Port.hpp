#ifndef COMM_SERIAL_PORT
#define COMM_SERIAL_PORT

/* Linux serial port headers */
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
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
  Port(std::string portName, unsigned int speed);
  void connect();
  void disconnect();
  bool isConnected();
  bool hasData();
  void push(const unsigned char* buffer, unsigned int length);
  void poll(unsigned char* buffer, unsigned int length);
  void lock();
  void unlock();

private:
  bool configure();

  int fileDescriptor = 0;
  std::string portName;
  unsigned int speed;
  bool connected;
  std::mutex threadLock;
  bool isLocked;
};

#endif
