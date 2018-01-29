#include <Arduino.h>
#include <Hub.h>

class Motors: public Controller{
public:
  void execute(Hub* hub, long* data, long length){
    // do something
    hub->emit(6, [1, 2, 3], 3);
  }
};

Hub* _hub;

void setup() {
  _hub = new Hub(controllers);
}

void loop() {
  _hub->poll();
}
