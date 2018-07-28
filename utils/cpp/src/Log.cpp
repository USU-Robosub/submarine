#include <Log.hpp>

Comm::Hub<std::string>* logHub = nullptr;
std::string logHandler = "";

void setEmitterForLogging(Comm::Hub<std::string>* hub, std::string handler){
  logHub = hub;
  logHandler = handler;
}

void writeToLog(std::vector<std::string> data){
  if(logHub == nullptr) return;
  logHub->emit(logHandler, data);
}


void GOOD(std::vector<std::string> data){
  std::vector<std::string> arrayVector;
  arrayVector.push_back("-1");
  arrayVector.insert(arrayVector.end(), data.begin(), data.end());
  writeToLog(arrayVector);
}

void INFO(std::vector<std::string> data){
  std::vector<std::string> arrayVector;
  arrayVector.push_back("0");
  arrayVector.insert(arrayVector.end(), data.begin(), data.end());
  writeToLog(arrayVector);
}

void WARN(std::vector<std::string> data){
  std::vector<std::string> arrayVector;
  arrayVector.push_back("1");
  arrayVector.insert(arrayVector.end(), data.begin(), data.end());
  writeToLog(arrayVector);
}

void ERROR(std::vector<std::string> data){
  std::vector<std::string> arrayVector;
  arrayVector.push_back("2");
  arrayVector.insert(arrayVector.end(), data.begin(), data.end());
  writeToLog(arrayVector);
}