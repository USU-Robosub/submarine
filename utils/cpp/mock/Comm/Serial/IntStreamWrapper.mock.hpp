#ifndef COMM_SERIAL_INT_STREAM_WRAPPER_MOCK
#define COMM_SERIAL_INT_STREAM_WRAPPER_MOCK

#include <Comm/Serial/IntStreamWrapper.hpp>

namespace Comm{
  namespace Serial{
    namespace Mock{
      class IntStreamWrapper;
    }
  }
}

class Comm::Serial::Mock::IntStreamWrapper : public Comm::IntStreamWrapper{
public:
  IntStreamWrapper()
    : Comm::IntStreamWrapper::IntStreamWrapper(nullptr), pollIndex(0) {}

  void lock() {}
  void unlock() {}

  int readInt() {
    return this->polled[pollIndex++];
  }

  void writeInt(int value){
    this->pushed.push_back(value);
  }

  bool hasData(){
    return pollIndex < polled.size();
  }

  std::vector<int> pushed;
  std::vector<int> polled;

private:
  int pollIndex;
};

#endif
