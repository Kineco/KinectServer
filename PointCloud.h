#ifndef PointCloud_h
#define PointCloud_h
#include <iostream>
#include <XnCppWrapper.h>

#include "DepthParam.h"
#include "json/json.h"

#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>



class PointCloud
{
public: 
    PointCloud(xn::DepthGenerator& depth);
    void update();
    Json::Value getPointCloud();
private:
    xn::DepthGenerator depth_;
    xn::DepthMetaData depthMD;
    DepthParam depthParam;

    
    int wid;
    int hei;
    boost::mutex mutex;
    
};
#endif
