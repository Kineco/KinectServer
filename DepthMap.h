#ifndef DepthMap_h
#define DepthMap_h
#include <iostream>
#include <XnCppWrapper.h>

#include "DepthParam.h"
#include "json/json.h"

#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>


class DepthMap
{
public: 
    explicit DepthMap(xn::DepthGenerator &depth);
    Json::Value getDepth(const int x,const int y);
    void update();

private:
    boost::mutex mutex;
    xn::DepthGenerator depth_;
    xn::DepthMetaData depthMD;

    int wid;
    int hei;
    
};
#endif

