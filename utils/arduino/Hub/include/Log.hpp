#ifndef LOG_HPP
#define LOG_HPP

#include <Emitter.hpp>

void setEmitterForLogging(Emitter* emitter, int32_t handler);

void writeToLog(const char* message, uint32_t length, int32_t level, int32_t* data);


void GOOD(const char* message, unsigned int length, int32_t* data);

template <typename... Ts>
void GOOD(const char* message, Ts... data){
  int32_t dataArray[] = { data... };
  writeToLog(message, sizeof...(data), -1, dataArray);
}

void INFO(const char* message, unsigned int length, int32_t* data);

template <typename... Ts>
void INFO(const char* message, Ts... data){
  int32_t dataArray[] = { data... };
  writeToLog(message, sizeof...(data), 0, dataArray);
}

void WARN(const char* message, unsigned int length, int32_t* data);

template <typename... Ts>
void WARN(const char* message, Ts... data){
  int32_t dataArray[] = { data... };
  writeToLog(message, sizeof...(data), 1, dataArray);
}

void ERROR(const char* message, unsigned int length, int32_t* data);

template <typename... Ts>
void ERROR(const char* message, Ts... data){
  int32_t dataArray[] = { data... };
  writeToLog(message, sizeof...(data), 2, dataArray);
}

void VERBOSE(const char* message, unsigned int length, int32_t* data);

template <typename... Ts>
void VERBOSE(const char* message, Ts... data){
  int32_t dataArray[] = { data... };
  writeToLog(message, sizeof...(data), 3, dataArray);
}

#endif
