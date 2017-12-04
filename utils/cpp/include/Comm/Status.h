#ifndef COMM_STATUS
#define COMM_STATUS

namespace Comm{
  enum class Status{
    Disconnected, Connecting, Ready, Error, Pending
  };
}

#endif
