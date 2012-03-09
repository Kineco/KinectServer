#ifndef KinectServer_FrontPoseDetector_h
#define KinectServer_FrontPoseDetector_h

#include "AbstractPoseDetector.h"



class FrontPoseDetector :
public AbstractPoseDetector {
    
public:
    FrontPoseDetector(Server *s, xn::Context &context);
    void detect();
private:
    bool calcPose();
    
    
};


#endif
