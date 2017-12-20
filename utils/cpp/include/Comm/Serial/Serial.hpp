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
#include <functional>
#include <Comm/BinaryPort.hpp>

namespace Comm{
  namespace Serial{
    class Port;
    struct RawData;
  }
}

class Comm::Serial::Port : public Comm::BinaryPort{
public:
  Port(std::string portName, unsigned int speed, unsigned short bufferLength);
  void connect();
  void disconnect();
  bool isConnected();
  bool hasData();
  void push(unsigned char* buffer, unsigned int length);
  void poll(unsigned char* buffer, unsigned int length);
  void lock();
  void unlock();

private:
  bool configure();

  int fileDescriptor = 0;
  std::string portName;
  unsigned int speed;
  bool connected;
  unsigned char* buffer;
  unsigned short bufferLength;
  std::mutex threadLock;
  bool isLocked;
};

#endif
