#ifdef ENABLE_OPEN_CV

#include <Vision/Livestream.hpp>
#define COLOR_BGR2HSV 40
#include <cstdio>
#include <fstream>

Vision::Livestream::Livestream(std::string path, int port, int quality)
  : imageStreamer(port, quality)
  , webcam()
  , loop(std::bind(&Vision::Livestream::doPeriodic, this, std::placeholders::_1), 1.0/2)
{
  cv::Mat3b original;
  original = cv::imread("../../../test/dice.jpg");
  cv::Mat3b hsv;
  #define COLOR_BGR2HSV 40
  cv::cvtColor(original, hsv, CV_BGR2HSV);
  
  cv::inRange(hsv, cv::Scalar(90 - 10, 70, 50), cv::Scalar(90 + 10, 255, 255), image);
  cameraId = Vision::Livestream::getVideoId(path);
  if(cameraId<0 || !webcam.open(cameraId))
  {
    throw "Camera not available on path "+path+" id:"+std::to_string(cameraId)+"!";
  } else {
    loop.start();
  }
}

Vision::Livestream::~Livestream(){
  loop.join();
}

int Vision::Livestream::getVideoId(std::string path){
  std::string filename = std::tmpnam(nullptr);
  system((
  "cams=\"$(ls /dev/video*)\";"
  "for camera in $cams; do"
  " path=\"$(udevadm info --query=all $camera | grep -m 1 'P:' | cut -c3-)\";"
  	"id=\"$(basename $(dirname $(dirname $path)) | cut -d: -f1)\";"
  	"echo \"$id,$(basename $camera | cut -c6-)\";"
  "done > " + filename).c_str()
  );
  std::ifstream file ( filename.c_str() );
  std::string fpath;
  std::string fid;
  while ( file.good() )
  {
     getline ( file, fpath, ',' );
     getline ( file, fid );
     if(fpath==path)
      return std::stoi(fid);
  }
  return -1;
}

void Vision::Livestream::doPeriodic(double deltaTime){
  cv::Mat3b frame;
  webcam.read(frame);
  if( frame.empty() )
  {
    webcam.open(cameraId);
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
  cv::Mat3b hsv;
  cv::cvtColor(frame, hsv, COLOR_BGR2HSV);
  
  cv::Mat1b mask; 
  cv::inRange(hsv, cv::Scalar(90 - 45, 70, 50), cv::Scalar(90, 255, 255), mask);
  
  // cv::Mat match;
  // int result_cols =  mask.cols - image.cols + 1;
  // int result_rows = mask.rows - image.rows + 1;

  // match.create( result_rows, result_cols, CV_32FC1 );

  // cv::matchTemplate( mask, image, match, CV_TM_SQDIFF ); // < from website
  // cv::normalize( match, match, 0, 225, cv::NORM_MINMAX, -1, cv::Mat() );

  // /// Localizing the best match with minMaxLoc
  // double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
  // cv::Point matchLoc;

  // cv::minMaxLoc( match, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );

  // /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
  // // if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
  // //   { matchLoc = minLoc; }
  // // else
  // //   { matchLoc = maxLoc; }
  // matchLoc = minLoc;

  // /// Show me what you got
  // //cv::rectangle( frame, matchLoc, cv::Point( matchLoc.x + image.cols , matchLoc.y + image.rows ), cv::Scalar::all(0), 2, 8, 0 );
  // image.copyTo(mask(cv::Rect(matchLoc.x,matchLoc.y,image.cols, image.rows)));
  // // end

  cv::putText(hsv, std::to_string(1/deltaTime), cv::Point(30,30),    cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0,0,0), 1, CV_AA);
  imageStreamer.serveFrame(hsv);
}

#endif
