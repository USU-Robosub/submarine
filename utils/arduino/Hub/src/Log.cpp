#include <Log.hpp>

Emitter* logEmitter = nullptr;
int32_t logHandler = 0;

void setEmitterForLogging(Emitter* emitter, int32_t handler){
  logEmitter = emitter;
  logHandler = handler;
}

void writeToLog(const char* message, unsigned int length, int level, int32_t* data){
  if(logEmitter == nullptr) return;
  unsigned int messageLength = 0;
  while(message[messageLength++] != 0){}
  messageLength--; // don't count null
  unsigned int compressedMessageLength = (messageLength + 3) / 4;
  unsigned int totalLength = 5 + 1 + 2 + compressedMessageLength + length;
  int32_t* logData = new int32_t[totalLength];
  // add log header
  logData[0] = 0;
  logData[1] = 1;
  logData[2] = 2;
  logData[3] = 3;
  logData[4] = 4;
  logData[5] = level;
  // add message length
  logData[6] = compressedMessageLength;
  logData[7] = messageLength;
  // add message
  for(unsigned int i = 0; i < compressedMessageLength; i++){
    logData[8 + i] = 0;
  }
  for(unsigned int i = 0; i < messageLength; i++){
    logData[8 + (i / 4)] = logData[8 + (i / 4)] | (message[i] << ((i % 4) * 8));
  }
  // add data
  for(unsigned int i = 0; i < length; i++){
    logData[8 + compressedMessageLength + i] = data[i];
  }
  // send message
  logEmitter->emit(logHandler, logData, totalLength);
  delete logData;
}

void GOOD(const char* message, unsigned int length, int32_t* data){
  writeToLog(message, length, -1, data);
}

void INFO(const char* message, unsigned int length, int32_t* data){
  writeToLog(message, length, 0, data);
}

void WARN(const char* message, unsigned int length, int32_t* data){
  writeToLog(message, length, 1, data);
}

void ERROR(const char* message, unsigned int length, int32_t* data){
  writeToLog(message, length, 2, data);
}

void VERBOSE(const char* message, unsigned int length, int32_t* data){
  writeToLog(message, length, 3, data);
}