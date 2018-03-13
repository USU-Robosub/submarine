#ifdef ENABLE_OPEN_CV

#ifndef STREAMEYE_H
#define STREAMEYE_H

#define SEPERATOR "--SEPERATOR--"
#define APP_NAME "streameye"

#include <Threading/ProcessChild.hpp>
#include <opencv2/opencv.hpp>
#include <ctime>
#include <chrono>

class StreamEye {
public:
  StreamEye(unsigned int port, int quality);
  void overlayTime(int r, int g, int b);
  void serveFrame(cv::Mat frame);
  void stopOverlay();
  StreamEye & operator=(const StreamEye&) = delete;
  StreamEye(const StreamEye&) = delete;

private:
  Threading::ProcessChild streamEyeApp;
  std::vector<int> compressionConfig;
  int timeR, timeG, timeB;
  bool showTime;
};

#endif

#endif
