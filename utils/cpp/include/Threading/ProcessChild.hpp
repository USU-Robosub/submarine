#ifndef THREADING_PROCESS_CHILD
#define THREADING_PROCESS_CHILD

#include <sys/wait.h>
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

namespace Threading{
  class ProcessChild;
}

class Threading::ProcessChild {
public:
  ProcessChild(std::string command, std::vector<std::string> arguments);
  ~ProcessChild();
  ProcessChild & operator=(const ProcessChild&) = delete;
  ProcessChild(const ProcessChild&) = delete;
  int read(unsigned char* buffer, std::size_t length);
  int write(const unsigned char* buffer, std::size_t length);

private:
  void parentProcess(pid_t pid);
  void childProcess(std::string command, std::vector<std::string> arguments);
  static char* convertArguments(const std::string& s);
  int parentToChild, childFromParent;
  int parentFromChild, childToParent;
  pid_t childPID;
};

#endif
