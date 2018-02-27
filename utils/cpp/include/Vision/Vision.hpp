#ifdef ENABLE_OPEN_CV

#ifndef VISION_H
#define VISION_H

#include <Timing/PeriodicLoop.h>
#include <opencv2/opencv.hpp>
#include <Vision/StreamEye.h>

class Vision {
public:
  Vision();
  ~Vision();
private:
  void doPeriodic(double deltaTime);
  StreamEye imageStreamer;
  cv::VideoCapture webcam;
  Timing::PeriodicLoop loop;
};

#endif

#endif
