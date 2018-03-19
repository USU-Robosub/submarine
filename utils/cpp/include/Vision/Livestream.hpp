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
  Livestream();
  ~Livestream();
private:
  void doPeriodic(double deltaTime);
  StreamEye imageStreamer;
  cv::VideoCapture webcam;
  cv::VideoCapture image;
  Timing::PeriodicLoop loop;
};

#endif

#endif
