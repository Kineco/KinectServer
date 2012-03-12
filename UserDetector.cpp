#include <iostream>
#include <sstream>
#include "config.h"

#include "UserDetector.h"


UserDetector::UserDetector(Server *s,xn::Context &context)
:AbstractUserDetector(context)
{
    s_ = s;
    ver_ = "1.0";
    type_ = "user";
    
}

void UserDetector::sendUserData(std::string action,XnUserID userID)
{
    std::stringstream output;
    output << "{\"kineco\": " << std::endl;
    output <<   "{\"config\": ";
    output <<       "{\"version\": " << ver_ << "," << std::endl;
    output <<       " \"type\": \"" << type_ << "\"}," << std::endl;
    output <<   " \"user\": " << std::endl;
    output <<       "{\"action\": \"" << action << "\"," << std::endl;
    output <<       " \"id\": \"" << userID << "\"}" << std::endl; 
    output <<   "}" << std::endl;
    output << "}" << std::endl;
    
    std::cout << output.str() << std::endl;
    s_->broadcastText(output.str());
}


void UserDetector::onNewUser(xn::UserGenerator& userGen, XnUserID userID)
{
    AbstractUserDetector::onNewUser(userGen, userID);
    sendUserData("detect",userID);
}
void UserDetector::onLostUser(xn::UserGenerator& userGen, XnUserID userID)
{
    AbstractUserDetector::onLostUser(userGen,userID);
    sendUserData("lost",userID);
     
}
void UserDetector::onCalibrationStart(xn::SkeletonCapability& cap, XnUserID userID)
{
    AbstractUserDetector::onCalibrationStart(cap, userID);
}
void UserDetector::onCalibrationEnd(xn::SkeletonCapability& cap, XnUserID userID,XnBool success)
{
    AbstractUserDetector::onCalibrationEnd(cap,userID, success);
    if(success)
    {
        sendUserData("calibration", userID);
    }
}

void UserDetector::onPoseStart(xn::PoseDetectionCapability& cap, const XnChar* pose, XnUserID userID)
{
    AbstractUserDetector::onPoseStart(cap,pose,userID);
}
void UserDetector::onPoseEnd(xn::PoseDetectionCapability& cap, const XnChar* pose, XnUserID userID)
{
    AbstractUserDetector::onPoseEnd(cap,pose,userID);
}
