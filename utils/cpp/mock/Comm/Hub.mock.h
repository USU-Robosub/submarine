#ifndef COMM_HUB_MOCK
#define COMM_HUB_MOCK

#include "Comm/Hub.h"

namespace Comm{
  class TestHub : public Hub{
  public:
    TestHub() {};

    int readInt() {

    }

    void writeInt(int value) {
      lastIntWrite = value;
    }

    void lock() {
      didLock = true;
    }

    void unlock(){
      didUnlock = true;
    }

    int intToRead = 0;
    int lastIntWrite = 0;
    bool didLock = false;
    bool didUnlock = false;
  };
}

#endif
