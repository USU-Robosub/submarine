#ifndef COMM_SERIAL_PORT_FINDER
#define COMM_SERIAL_PORT_FINDER

#include <string>

namespace Comm{
  namespace Serial{
    namespace PortFinder{
      std::string findByPath(std::string path);
    }
  }
}

#endif
