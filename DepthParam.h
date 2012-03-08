#ifndef DepthParam_h
#define DepthParam_h
#include <iostream>
#include <XnCppWrapper.h>


class DepthParam
{

public:
     void setParam(const xn::DepthGenerator &depth);
     void setParam(const xn::DepthMetaData &depthMD);
     bool depth2point(const XnDepthPixel* depthMap,XnPoint3D *points);
      
public:
     XnUInt64 focalLength;
     XnDouble pixelSize;
     int uSize, vSize;
     int uCenter;
     int vCenter;
};
#endif
