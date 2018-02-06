#include <iostream>

#include <Comm/Hub.hpp>
#include <Comm/TCP/Bridge.hpp>
#include <Comm/TCP/Stream.hpp>
#include <Comm/TCP/Port.hpp>
#include <Comm/TCP/SingleClientServer.hpp>
#include <Streameye.h>
#include <opencv2/opencv.hpp>
#include <Timing/PeriodicLoop.h>

#include <thread>
#include <chrono>


int main(){
  StreamEye imageStreamer(3001, 50);
  imageStreamer.overlayTime(0, 255, 0);
  cv::VideoCapture cap;
  // open the default camera, use something different from 0 otherwise;
  // Check VideoCapture documentation.
  if(!cap.open(0))
  {
    std::cerr << "Could not open video" << std::endl;
    exit(EXIT_FAILURE);
  }
  cv::Mat frame;
  Timing::PeriodicLoop loop([&frame, &cap, &imageStreamer, &loop](double deltaTime){
    cap.read(frame);
    if( frame.empty() )
    {
      cap.open(0);
      cap.read(frame);
      if( frame.empty() )
      {
        std::cerr << "Could not restart video." << std::endl;
        loop.stop();
        return;
      }; // end of video stream
    }; // end of video stream
    //cv::resize(frame, frame, cv::Size(640/4, 360/4));
    cv::putText(frame, std::to_string(deltaTime), cv::Point(30,30),    cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(150,150,150), 1, CV_AA);
    imageStreamer.serveFrame(frame);
  }, 1.0/20);

  loop.start();

  loop.join();
  // the camera will be closed automatically upon exit
  // cap.close();

  return 0;
}
