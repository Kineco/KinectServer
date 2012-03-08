#ifndef RgbImage_h
#define RgbImage_h
#include <iostream>
#include <vector>
#include <stdexcept>
#include <XnCppWrapper.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <boost/thread.hpp>
#include "DepthParam.h"
#include "json/json.h"
#include "PolarSSL/base64.h"


class RgbImage
{
  public: 
    explicit RgbImage(xn::ImageGenerator &image);
    
    Json::Value getRgbData();
    std::vector<unsigned char> getJpegImage();
    std::vector<unsigned char> getRgbBinary();
    
    void update();

    
  private:

    
    int wid;
    int hei;
    xn::ImageGenerator image_;
    xn::ImageMetaData imageMD;
    //xn::Context context_;
    boost::mutex mutex;
};
#endif

