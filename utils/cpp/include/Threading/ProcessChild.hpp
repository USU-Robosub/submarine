#ifndef STREAMEYE_H
#define STREAMEYE_H

#define SEPERATOR "--SEPERATOR--"

#include <vector>

namespace Threading{
  class ProcessChild;
}

class ProcessChild {
public:
  ProcessChild();
  void writeJpeg(std::vector<uchar> image);
  ~ProcessChild();
  ProcessChild & operator=(const ProcessChild&) = delete;
  ProcessChild(const ProcessChild&) = delete;

private:
  void parentProcess(pid_t pid);
  void childProcess();
  int m_fdIn[2];
  int m_fdOut[2];
};

#endif
