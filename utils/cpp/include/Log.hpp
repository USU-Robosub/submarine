#ifndef LOG_HPP
#define LOG_HPP

#include <Comm/Hub.hpp>
#include <string>

void setEmitterForLogging(Comm::Hub<std::string>* hub, std::string handler);

void writeToLog(std::vector<std::string> data);


void GOOD(std::vector<std::string> data);

template <typename... Ts>
void GOOD(Ts... data){
  GOOD({ data... });
}

void INFO(std::vector<std::string> data);

template <typename... Ts>
void INFO(Ts... data){
  INFO({ data... });
}

void WARN(std::vector<std::string> data);

template <typename... Ts>
void WARN(Ts... data){
  WARN({ data... });
}

void ERROR(std::vector<std::string> data);

template <typename... Ts>
void ERROR(Ts... data){
  ERROR({ data... });
}

#endif
