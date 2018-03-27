#ifdef ENABLE_OPEN_CV

#include <Vision/Livestream.hpp>

Vision::Livestream::Livestream()
  : imageStreamer(8000, 100)
  , webcam()
  , loop(std::bind(&Vision::Livestream::doPeriodic, this, std::placeholders::_1), 1.0/2)
{
  image = cv::imread("../../../test/dice.jpg");
  cv::Mat bgr[3];   //destination array
  cv::split(image,bgr);//split source
  //Note: OpenCV uses BGR color order
  bgr[0]=bgr[1]=cv::Mat::zeros(cv::Size(image.cols, image.rows), CV_8U);

  cv::merge(bgr,3,image);

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

  // cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);

  // Start

  cv::Mat bgr[3];   //destination array
  cv::split(frame,bgr);//split source
  //Note: OpenCV uses BGR color order
  bgr[0]=bgr[1]=cv::Mat::zeros(cv::Size(frame.cols, frame.rows), CV_8U);
  cv::merge(bgr,3,frame);
  cv::Mat match;
  int result_cols =  frame.cols - image.cols + 1;
  int result_rows = frame.rows - image.rows + 1;

  match.create( result_rows, result_cols, CV_32FC1 );

  cv::matchTemplate( frame, image, match, CV_TM_SQDIFF ); // < from website
  cv::normalize( match, match, 0, 225, cv::NORM_MINMAX, -1, cv::Mat() );

  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
  cv::Point matchLoc;

  cv::minMaxLoc( match, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );

  /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
  // if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
  //   { matchLoc = minLoc; }
  // else
  //   { matchLoc = maxLoc; }
  matchLoc = minLoc;

  /// Show me what you got
  //cv::rectangle( frame, matchLoc, cv::Point( matchLoc.x + image.cols , matchLoc.y + image.rows ), cv::Scalar::all(0), 2, 8, 0 );
  image.copyTo(frame(cv::Rect(matchLoc.x,matchLoc.y,image.cols, image.rows)));
  // end

  cv::putText(frame, std::to_string(deltaTime), cv::Point(30,30),    cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0,0,0), 1, CV_AA);
  imageStreamer.serveFrame(frame);
}

#endif
