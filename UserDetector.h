#ifndef echoServer_UserDetector_h
#define echoServer_UserDetector_h


#include <iostream>
#include "AbstractUserDetector.h"
#include "Websocket/Server.h"

class UserDetector:public AbstractUserDetector {
private:    
    Server *s_;
    std::string ver_;
    std::string type_;
    
public:
    explicit UserDetector(Server *s,xn::Context &context);

    void startCalibration(XnUserID userID);
    void sendUserData(std::string action, XnUserID userID);
    
    void onNewUser(xn::UserGenerator& userGen, XnUserID);
    void onLostUser(xn::UserGenerator& userGen, XnUserID userID);
    
    void onCalibrationStart(xn::SkeletonCapability& cap, XnUserID userID);
    void onCalibrationEnd(xn::SkeletonCapability& cap, XnUserID userID,XnBool success);
    
    void onPoseStart(xn::PoseDetectionCapability& cap, const XnChar* pose, XnUserID userID);
    void onPoseEnd(xn::PoseDetectionCapability& cap, const XnChar* pose, XnUserID userID);
    
};




#endif
