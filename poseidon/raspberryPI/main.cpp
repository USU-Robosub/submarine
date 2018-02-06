#include <iostream>

#include <Comm/Hub.hpp>
#include <Comm/TCP/Bridge.hpp>
#include <Comm/TCP/Stream.hpp>
#include <Comm/TCP/Port.hpp>
#include <Comm/TCP/SingleClientServer.hpp>
#include <Streameye.h>
#include <opencv2/opencv.hpp>

#include <thread>
#include <chrono>


int main(){
  StreamEye imageStreamer;
  cv::VideoCapture cap;
  // open the default camera, use something different from 0 otherwise;
  // Check VideoCapture documentation.
  if(!cap.open("foho.mp4"))
  {
    std::cerr << "Could not open video" << std::endl;
    exit(EXIT_FAILURE);
  }
  for(;;)
  {
        cv::Mat frame;
        cap >> frame;
        if( frame.empty() )
        {
          cap.open("foho.mp4");
          cap >> frame;
          if(frame.empty())
            break;

        }; // end of video stream
        std::vector<unsigned char> image;
        cv::imencode(".jpg", frame, image);
        //imageStreamer.writeJpeg(image);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  // the camera will be closed automatically upon exit
  // cap.close();

  return 0;
}
