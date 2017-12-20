#ifndef COMM_STREAM_MOCK
#define COMM_STREAM_MOCK

#include <Comm/Stream.hpp>

namespace Comm{
  namespace Mock{
    template<class T>
    class Stream;
  }
}

template<class T>
class Comm::Mock::Stream : public Comm::Stream<T>{
public:
  Stream()
    : pollIndex(0) {}

  T poll() {
    return this->polled[pollIndex++];
  }

  void push(T value){
    this->pushed.push_back(value);
  }

  bool hasData(){
    return pollIndex < polled.size();
  }

  void lock(){ }

  void unlock(){ }

  std::vector<T> pushed;
  std::vector<T> polled;

private:
  int pollIndex;
};

#endif
