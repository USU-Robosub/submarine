#include <Arduino.h>

long readSerial(){
  long n = Serial.read();
  n += Serial.read() << 1;
  n += Serial.read() << 2;
  n += Serial.read() << 3;
  return n;
}

void writeCommand(long name, long* data, long length){
  long null = 0;
  Serial.write((char*)&null, 4);
  Serial.write((char*)&name, 4);
  Serial.write((char*)&length, 4);
  for(long i = 0; i < length; ++i){
    Serial.write((char*)&data[i], 4);
  }
}

class Motors: public Controller{
public:
  void execute(long* data, long length){
    // do something
    writeCommand(6, [1, 2, 3], 3);
  }
};

void runCommand(Controller* controllers){
  long null = readSerial();
  if(null != 0){
    // Error, message misaligned
  }
  long command = readSerial();
  long length = readSerial();
  long* data = new long[length];
  for(unsigned long i = 0; i < length; ++i){
    data[i] = readSerial();
  }
  controllers[command]->execute(data, length);
  delete data;
}

void setup() {
  Serial.begin(9600);
  while(!Serial){}
}

void loop() {
  runCommand(controllers);
}
