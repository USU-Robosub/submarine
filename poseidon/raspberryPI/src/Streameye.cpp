#include <Streameye.h>
int ofile = -1;

StreamEye::StreamEye(unsigned int port, int quality)
  : streamEyeApp(APP_NAME,
      {"-q", "-p", std::to_string(port), "-s", SEPERATOR}
    ),
    compressionConfig({CV_IMWRITE_JPEG_QUALITY, quality}),
    timeR(0), timeG(0), timeB(0),
    showTime(false) {}

void StreamEye::serveFrame(cv::Mat frame) {
  if(this->showTime){
    std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    cv::putText(frame, std::ctime(&t), cv::Point(30, frame.rows - 30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(this->timeB,this->timeG,this->timeR), 1, CV_AA);
  }
  std::vector<unsigned char> image;
  cv::imencode(".jpg", frame, image, this->compressionConfig);
  this->streamEyeApp.write((unsigned char*)SEPERATOR, sizeof(SEPERATOR)-1);
  this->streamEyeApp.write((unsigned char*)&image[0], sizeof(unsigned char)*image.size());
}

void StreamEye::overlayTime(int r, int g, int b){
  this->timeR = r;
  this->timeG = g;
  this->timeB = b;
  this->showTime = true;
}

void StreamEye::stopOverlay(){
  this->showTime = false;
}
