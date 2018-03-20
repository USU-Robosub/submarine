#ifdef ENABLE_OPEN_CV

#include <Vision/Livestream.hpp>

Vision::Livestream::Livestream()
  : imageStreamer(8000, 100)
  , webcam()
  , loop(std::bind(&Vision::Livestream::doPeriodic, this, std::placeholders::_1), 1.0/15)
{
  image = cv::imread("../../test/test.jpg", 0);
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

  cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);

  std::cout << frame.type() << std::endl;
  std::cout << image.type() << std::endl;

  // Start

  cv::Mat match;
  int result_cols =  frame.cols - image.cols + 1;
  int result_rows = frame.rows - image.rows + 1;

  match.create( result_rows, result_cols, CV_32FC1 );

  cv::matchTemplate( frame, image, match, CV_TM_SQDIFF ); // < from website
  cv::normalize( match, match, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );

  // end

  cv::putText(match, std::to_string(deltaTime), cv::Point(30,30),    cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0,0,0), 1, CV_AA);
  imageStreamer.serveFrame(match);
}

#endif
