#ifndef SERIAL_MOCK
#define SERIAL_MOCK

namespace Mock{
  class Serial;
}

class Mock::Serial{
public:
  void begin(int port) {}
  explicit operator bool() const { return true; }
  int read() {}
  void write(char* buffer, long length){}
};

static Mock::Serial Serial;

#endif
