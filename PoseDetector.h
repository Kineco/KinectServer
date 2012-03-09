#ifndef WebSocketServer_PoseDetector_h
#define WebSocketServer_PoseDetector_h

#include "InterfacePoseDetector.h"
#include "AbstractUserDetector.h"

#include "WebSocket/Server.h"

#include <XnCppWrapper.h>
#include "Vector3D.hpp"

class PoseDetector: 
    public InterfacePoseDetector, 
    public AbstractUserDetector
{   
    
    int posingTime; //Poseの持続時間
    bool posing; //Poseが継続しているか
    int lostTime; //PoseがLostしてからの持続時間。
    std::string poseName;
    std::string prevPoseName;
    
    Server *server;
    std::string type_;
    std::string ver_;
    
public:
    PoseDetector(xn::Context &context, Server *s);
    ~PoseDetector();
    void detect();
private:    
    bool calcPose();
    void KeepTime();
    Vector3D getVector(XnSkeletonJoint joint1, XnSkeletonJoint joint2);
    
    void detectPose(std::string pose);
    void lostPose(std::string pose);
    
    
};


#endif
