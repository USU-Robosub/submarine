#ifndef COMM_BRIDGE_MOCK
#define COMM_BRIDGE_MOCK

#include <Comm/Bridge.hpp>
#include <vector>

namespace Comm{
  namespace Mock{
    template<class T>
    class Bridge;
  }
}

template<class T>
class Comm::Mock::Bridge : public Comm::Bridge<T>{
public:
  void send(std::vector<T> data);
  std::queue<std::vector<T>> receive();

  std::vector<std::vector<T>> sent;
  std::queue<std::vector<T>> received;
};

template<class T>
void Comm::Mock::Bridge<T>::send(std::vector<T> data){
  sent.push_back(data);
}

template<class T>
std::queue<std::vector<T>> Comm::Mock::Bridge<T>::receive(){
  return received;
}

#endif
