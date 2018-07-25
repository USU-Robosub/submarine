#include <Comm/Serial/PortFinder.hpp>

#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <sstream>
#include <stdexcept>

namespace Comm{
  namespace Serial{
    namespace PortFinder{
      std::vector<std::string> readDirectory(std::string name);
      std::vector<std::string> split(std::string s, char delim);
    }
  }
}

#define RASPBERRY_PI_USB_PORT_ID 1 // Example: platform-bcm2708_usb-usb-0:1.3:1.0-port0 -> 1.3

std::string Comm::Serial::PortFinder::findByPath(std::string path){
  std::vector<std::string> devices = Comm::Serial::PortFinder::readDirectory("/dev/serial/by-path");
  for(unsigned int i = 0; i < devices.size(); ++i){
    std::vector<std::string> pathParts = Comm::Serial::PortFinder::split(devices[i], ':');
    if(pathParts[RASPBERRY_PI_USB_PORT_ID] == path){
      return devices[i];
    }
  }
  throw std::runtime_error("Failed to find serial port from path \"" + path + "\".");
}

std::vector<std::string> Comm::Serial::PortFinder::readDirectory(std::string name){
  DIR* dirp = opendir(name.c_str());
  if(dirp == NULL){
    throw std::runtime_error("Could not open directory \"" + name + "\"");
  }
  struct dirent* dp;
  std::vector<std::string> files;
  while ((dp = readdir(dirp)) != NULL) {
    files.push_back(dp->d_name);
  }
  closedir(dirp);
  return files;
}

std::vector<std::string> Comm::Serial::PortFinder::split(std::string s, char delim) {
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> tokens;
  while (getline(ss, item, delim)) {
    tokens.push_back(item);
  }
  return tokens;
}
