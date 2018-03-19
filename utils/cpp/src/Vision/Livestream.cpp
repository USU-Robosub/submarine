#ifdef ENABLE_OPEN_CV

#include <Vision/Livestream.hpp>

Vision::Livestream::Livestream()
  : imageStreamer(8000, 100)
  , webcam()
  , image()
  , loop(std::bind(&Vision::Livestream::doPeriodic, this, std::placeholders::_1), 1.0/15)
{
  if(!image.open('../../test/test.jpg')){
    std::cerr << "Could not open template" << std::endl;
    exit(EXIT_FAILURE);
  }
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

  // Start

  cv::Mat match;
  matchTemplate( frame, image, match, CV_TM_SQDIFF ); // < from website
  normalize( match, match, 0, 1, NORM_MINMAX, -1, Mat() );

  // end

  cv::putText(match, std::to_string(deltaTime), cv::Point(30,30),    cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0,0,0), 1, CV_AA);
  imageStreamer.serveFrame(match);
}

#endif
