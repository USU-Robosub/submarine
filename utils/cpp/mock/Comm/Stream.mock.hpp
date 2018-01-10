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
  Stream();
  T poll();
  void push(T value);
  bool hasData();
  void lock();
  void unlock();

  std::vector<T> pushed;
  std::vector<T> polled;

private:
  int pollIndex;
};

template<class T>
Comm::Mock::Stream<T>::Stream()
  : pollIndex(0) {}

template<class T>
T Comm::Mock::Stream<T>::poll() {
  return this->polled[pollIndex++];
}

template<class T>
void Comm::Mock::Stream<T>::push(T value){
  this->pushed.push_back(value);
}

template<class T>
bool Comm::Mock::Stream<T>::hasData(){
  return pollIndex < polled.size();
}

template<class T>
void Comm::Mock::Stream<T>::lock(){ }

template<class T>
void Comm::Mock::Stream<T>::unlock(){ }

#endif
