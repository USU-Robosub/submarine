#ifdef ENABLE_OPEN_CV

#include <Vision/Vision.hpp>

Vision::Vision()
  : imageStreamer()
  , webcam()
  , loop(std::bind(&Vision::doPeriodic, this, std::placeholders::_1), 1.0/15)
{
  std::cout << "Vision constructor" << std::endl;
  // open the default camera, use something different from 0 otherwise;
  if(!webcam.open(0))
  {
    std::cerr << "Could not open video" << std::endl;
    exit(EXIT_FAILURE);
  }
  loop.start();
}


Vision::~Vision(){
  loop.join();
}

void Vision::doPeriodic(double deltaTime){
  std::vector<int> compression_params;
  compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
  compression_params.push_back(100);
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
    std::vector<unsigned char> image;
    //cv::resize(frame, frame, cv::Size(640/4, 360/4));
    cv::putText(frame, std::to_string(deltaTime), cv::Point(30,30),    cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0,0,0), 1, CV_AA);
    cv::imencode(".jpg", frame, image, compression_params);
    imageStreamer.writeJpeg(image);
}

#endif
