#ifdef ENABLE_OPEN_CV

#ifndef STREAMEYE_H
#define STREAMEYE_H

#define SEPERATOR "--SEPERATOR--"

#include <sys/wait.h>
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <errno.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

class StreamEye {
public:
  StreamEye();
  void writeJpeg(std::vector<unsigned char> image);
  ~StreamEye();
  StreamEye & operator=(const StreamEye&) = delete;
  StreamEye(const StreamEye&) = delete;

private:
  void childProcess();
  int m_fdIn[2];
  int m_fdOut[2];
  int m_PID;
};

#endif

#endif
