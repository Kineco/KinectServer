#ifndef DepthImage_h
#define DepthImage_h
#include <iostream>
#include <stdexcept>
#include <vector>

#include <XnCppWrapper.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "DepthParam.h"
#include "json/json.h"
#include "PolarSSL/base64.h"

#include <boost/thread.hpp>

typedef std::vector<float> depth_hist;

class DepthImage
{
  public: 
    DepthImage(xn::DepthGenerator &depth);
    Json::Value getDepthImageData();
    std::vector<unsigned char> getDepthImage();
    void update();
    depth_hist getDepthHistgram();
       
private:
    xn::DepthMetaData depthMD;
    xn::ImageMetaData imageMD;
    xn::DepthGenerator depth_;
    int wid;
    int hei;
    
    boost::mutex mutex;
    
};
#endif

