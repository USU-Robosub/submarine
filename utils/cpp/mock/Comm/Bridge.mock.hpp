#ifndef COMM_BRIDGE_MOCK
#define COMM_BRIDGE_MOCK

#include <Comm/Bridge.h>
#include <vector>

namespace Comm{
  namespace Mock{
    class TestBridge : public Bridge<int>{

    };

    class StringBridge : public Bridge<std::string>{
    public:
      void send(std::vector<std::string> data){
        sent.push_back(data);
      }

      std::queue<std::vector<std::string>> receive(){
        return received;
      }

      std::vector<std::vector<std::string>> sent;
      std::queue<std::vector<std::string>> received;
    };
  }
}

#endif