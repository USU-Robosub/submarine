#ifdef ENABLE_OPEN_CV

#ifndef VISION_H
#define VISION_H

#include <Timing/PeriodicLoop.h>
#include <opencv2/opencv.hpp>
#include <Vision/Streameye.hpp>

namespace Vision{
  class Livestream;
}

class Vision::Livestream {
public:
  Livestream(std::string path, int port, int quality);
  ~Livestream();
private:
  int cameraId;
  static int getVideoId(std::string path);
  void doPeriodic(double deltaTime);
  StreamEye imageStreamer;
  cv::VideoCapture webcam;
  cv::Mat1b image;
  Timing::PeriodicLoop loop;
};

#endif

#endif
