#ifndef LOG_HPP
#define LOG_HPP

#include <Emitter.hpp>

#define INFO 0
#define WARN 1
#define ERROR 2

#define LOG(message, data, length, level) writeToLog(message, data, length, level)

void setEmitterForLogging(Emitter* emitter, int32_t handler);

void writeToLog(const char* message, int32_t* data, unsigned int length, unsigned int level);

#endif
