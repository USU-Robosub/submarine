#ifdef ENABLE_OPEN_CV

#include <Vision/Livestream.hpp>

Vision::Livestream::Livestream()
  : imageStreamer(8080, 100)
  , webcam()
  , loop(std::bind(&Vision::Livestream::doPeriodic, this, std::placeholders::_1), 1.0/15)
{
  if(!webcam.open(0))
  {
    std::cerr << "Could not open video" << std::endl;
    exit(EXIT_FAILURE);
  }
  loop.start();
}


Vision::Livestream::~Livestream(){
  loop.join();
}

void Vision::Livestream::doPeriodic(double deltaTime){
  cv::Mat frame;
  webcam.read(frame);
  if( frame.empty() )
  {
    webcam.open(0);
    webcam.read(frame);
    if( frame.empty() )
    {
      std::cerr << "Could not restart video." << std::endl;
      loop.stop();
      return;
    }; // end of video stream
  }; // end of video stream
  //cv::resize(frame, frame, cv::Size(640/4, 360/4));
  cv::putText(frame, std::to_string(deltaTime), cv::Point(30,30),    cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0,0,0), 1, CV_AA);
  imageStreamer.serveFrame(frame);
}

#endif
