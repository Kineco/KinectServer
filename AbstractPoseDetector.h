#ifndef WebSocketServer_AbstractPoseDetector_h
#define WebSocketServer_AbstractPoseDetector_h

#include <XnCppWrapper.h>
#include "WebSocket/Server.h"
#include "AbstractUserDetector.h"

#include "Vector3D.hpp"


class AbstractPoseDetector:
    public AbstractUserDetector
{
protected:
    int posingTime; //Poseの持続時間
    bool posing; //Poseが継続しているか
    int lostTime; //PoseがLostしてからの持続時間。
    std::string poseName;
    
    Server *server;
    std::string type_;
    std::string ver_;
    
    
public:
    AbstractPoseDetector( Server *s, xn::Context &context);
    
    Vector3D getVector(XnSkeletonJoint joint1, XnSkeletonJoint joint2);
    void detectPose();
    void lostPose();
    
    virtual void detect()=0;
};

#endif
