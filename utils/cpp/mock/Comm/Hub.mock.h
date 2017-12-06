#ifndef COMM_HUB_MOCK
#define COMM_HUB_MOCK

#include <stack>
#include "Comm/Hub.h"

namespace Comm{
  class TestHub : public Hub{
  public:
    TestHub() {};

    int readInt() {

    }

    void writeInt(int value) {
      writeQueue.push(value);
    }

    void lock() {
      didLock = true;
    }

    void unlock(){
      didUnlock = true;
    }

    int intToRead = 0;
    std::stack<int> writeQueue;
    bool didLock = false;
    bool didUnlock = false;
  };
}

#endif
