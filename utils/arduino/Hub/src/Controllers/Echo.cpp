#include <Controllers/Echo.hpp>

Controllers::Echo::Echo(int32_t returnName)
  : returnName(returnName) {}

void Controllers::Echo::execute(Emitter* hub, int32_t* data, int32_t length){
  hub->emit(this->returnName, data, length);
}
