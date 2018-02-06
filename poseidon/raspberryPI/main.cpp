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
  StreamEye imageStreamer;
  cv::VideoCapture cap;
  // open the default camera, use something different from 0 otherwise;
  // Check VideoCapture documentation.
  if(!cap.open(0))
  {
    std::cerr << "Could not open video" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::vector<int> compression_params;
  compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
  compression_params.push_back(100);
  cv::Mat frame;
  Timing::PeriodicLoop loop([&frame, &cap, &compression_params, &imageStreamer, &loop](double deltaTime){
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
    std::vector<unsigned char> image;
    //cv::resize(frame, frame, cv::Size(640/4, 360/4));
    cv::putText(frame, std::to_string(deltaTime), cv::Point(30,30),    cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0,0,0), 1, CV_AA);
    cv::imencode(".jpg", frame, image, compression_params);
    imageStreamer.writeJpeg(image);
  }, 1.0/20);

  loop.start();

  loop.join();
  // the camera will be closed automatically upon exit
  // cap.close();

  return 0;
}
