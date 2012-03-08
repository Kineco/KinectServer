#include <iostream>

#include "AbstractUserDetector.h"
#include "UserDetector.h"
//コールバック用のメソッド群
void XN_CALLBACK_TYPE AbstractUserDetector::s_NewUser(xn::UserGenerator& userGen, XnUserID userID, void* that)
{
    static_cast<AbstractUserDetector*>(that)->onNewUser(userGen, userID);
}
void XN_CALLBACK_TYPE AbstractUserDetector::s_LostUser(xn::UserGenerator& userGen, XnUserID userID, void* that)
{
    static_cast<AbstractUserDetector*>(that)->onLostUser(userGen, userID);
}

void XN_CALLBACK_TYPE AbstractUserDetector::s_CalibrationStart(xn::SkeletonCapability& cap, XnUserID userID, void* that)
{
    static_cast<AbstractUserDetector*>(that)->onCalibrationStart(cap, userID);
}
void XN_CALLBACK_TYPE AbstractUserDetector::s_CalibrationEnd(xn::SkeletonCapability& cap, XnUserID userID, XnBool success, void* that)
{
    static_cast<AbstractUserDetector*>(that)->onCalibrationEnd(cap, userID, success);
}

void XN_CALLBACK_TYPE AbstractUserDetector::s_PoseStart(xn::PoseDetectionCapability& cap, const XnChar* pose, XnUserID userID, void* that)
{
    static_cast<AbstractUserDetector*>(that)->onPoseStart(cap, pose, userID);
}
void XN_CALLBACK_TYPE AbstractUserDetector::s_PoseEnd(xn::PoseDetectionCapability& cap, const XnChar* pose, XnUserID userId, void* that)
{
    static_cast<UserDetector*>(that)->onPoseEnd(cap, pose, userId);
}
//コールバックメソッドここまで

AbstractUserDetector::AbstractUserDetector(xn::Context &context)
:context_(context)
{
    tracking = false;
    tracking_user = 0;
    XnStatus rc;
    rc = context_.FindExistingNode(XN_NODE_TYPE_USER, user_gen);
    
    user_gen.Create(context_);
    
    rc = user_gen.IsCapabilitySupported(XN_CAPABILITY_SKELETON);
    if(rc!=XN_STATUS_OK)
    {
        std::cout << "userGenerator: " << std::endl;
       // throw std::runtime_error(xnGetStatusString(rc));
    }
    
    user_gen.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
    
    user_gen.RegisterUserCallbacks(&UserDetector::s_NewUser, &UserDetector::s_LostUser, this, user_handle);
    user_gen.GetSkeletonCap().RegisterCalibrationCallbacks(&UserDetector::s_CalibrationStart, &UserDetector::s_CalibrationEnd, this, calibration_handle);
    
    if(user_gen.GetSkeletonCap().NeedPoseForCalibration())
    {
        rc = user_gen.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION);
        if(rc!=XN_STATUS_OK)
        {
            //throw std::runtime_error(xnGetStatusString(rc));
        }
        pose = new XnChar[20];
        user_gen.GetSkeletonCap().GetCalibrationPose(pose);
        std::cout << "Caribration Pose=" << pose << std::endl;
        
        user_gen.GetPoseDetectionCap().RegisterToPoseCallbacks(&UserDetector::s_PoseStart, &UserDetector::s_PoseEnd, this, pose_handle);
        
    }else {
        pose = NULL;
        pose_handle = NULL;
    }
}

AbstractUserDetector::~AbstractUserDetector()
{
    delete[] pose;
}


bool AbstractUserDetector::isTracking()
{
    lock lk(mutex);
    return tracking;
}
XnUserID AbstractUserDetector::trackingUserID()
{
    lock lk(mutex);
    return tracking_user;
}

void AbstractUserDetector::startCalibration(XnUserID userID)
{
    if(pose)
        user_gen.GetPoseDetectionCap().StartPoseDetection(pose, userID);
    else
        user_gen.GetSkeletonCap().RequestCalibration(userID, TRUE);
}

void AbstractUserDetector::onNewUser(xn::UserGenerator& userGen, XnUserID userID)
{
    std::cout << "new user" << std::endl;
    std::cout << "-id:" << userID << std::endl;
    startCalibration(userID);
}
void AbstractUserDetector::onLostUser(xn::UserGenerator& userGen, XnUserID userID)
{
    std::cout << "lost user" << std::endl;
    std::cout << "-id:" << userID << std::endl;
    
    if(userID == tracking_user)
    {
        lock lk(mutex);
        tracking_user = 0;
        tracking = false;
    }
    
}

void AbstractUserDetector::onCalibrationStart(xn::SkeletonCapability& cap, XnUserID userID)
{
    std::cout << "CalibrationStart" << std::endl;
}
void AbstractUserDetector::onCalibrationEnd(xn::SkeletonCapability& cap, XnUserID userID,XnBool success)
{
    std::cout << "CalibrationEnd:" ;
    if(success)
    {
        std::cout << " Calibration Success" << std::endl;
        user_gen.GetSkeletonCap().StartTracking(userID);
        tracking = true;
        tracking_user = userID;
    }
    else{
        std::cout << " Calibration Failed" << std::endl;
        startCalibration(userID);
    }
}

void AbstractUserDetector::onPoseStart(xn::PoseDetectionCapability& cap, const XnChar* pose, XnUserID userID)
{
    user_gen.GetPoseDetectionCap().StopPoseDetection(userID);
    user_gen.GetSkeletonCap().RequestCalibration(userID, TRUE);
}
void AbstractUserDetector::onPoseEnd(xn::PoseDetectionCapability& cap, const XnChar* pose, XnUserID userID)
{
    
}
