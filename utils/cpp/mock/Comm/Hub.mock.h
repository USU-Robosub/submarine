#ifndef COMM_HUB_MOCK
#define COMM_HUB_MOCK

#include <vector>
#include "Comm/Hub.h"

namespace Comm{
  class TestHub : public Hub{
  public:
    TestHub() {};

    int readInt() {

    }

    void writeInt(int value) {
      writeQueue.push_back(value);
    }

    void lock() {
      didLock = true;
    }

    void unlock(){
      didUnlock = true;
    }

    int intToRead = 0;
    std::vector<int> writeQueue;
    bool didLock = false;
    bool didUnlock = false;
  };
}

#endif
