#ifndef COMM_BRIDGE
#define COMM_BRIDGE

#include <vector>
#include <queue>

namespace Comm{
  enum class MessageState;

  template<class T>
  struct Message;

  template<class T>
  class Bridge;
}

enum class Comm::MessageState {
  CHECK, NAME, LENGTH, DATA, ALIGNING
};

template<class T>
struct Comm::Message {
  T check;
  T name;
  T length;
  std::vector<T> data;
};

template<class T>
class Comm::Bridge{
public:
  virtual ~Bridge() {};
  virtual void send(std::vector<T> data) = 0;
  virtual std::queue<std::vector<T>> receive() = 0;
};

#endif
