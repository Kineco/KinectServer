#ifndef CalibratedUsers_h
#define CalibratedUsers_h
#include <iostream>
#include <map>

#include <XnCppWrapper.h>

#include "DepthParam.h"
#include "json/json.h"

#include <boost/thread.hpp>

#define MAX_USER_NUM 15
#define NUM_AVAILABLE_JOINTS 24 //OpenNIで規定されているSkeletonJointPostionの数
#define NUM_JOINTS 15 // Kinectで利用出来るSkeletonJointPositionの数





class CalibratedUsers
{
  public: 
    CalibratedUsers(xn::UserGenerator &user_);
    Json::Value getCalibratedUserIds();
    Json::Value getNumberOfCalibratedUsers();
    Json::Value getSkeletonJointPosition(const int id_, std::string ejoint_str);
    Json::Value getSkeletonById(const int id_);
    void update();

    
private:
    boost::mutex mutex;
    
    XnPoint3D skeletons[MAX_USER_NUM][NUM_AVAILABLE_JOINTS];
    XnUserID calibratedUsers[MAX_USER_NUM];
    int calibratedCount;
    xn::UserGenerator userGen_;
    std::map<XnSkeletonJoint,std::string> parts_;
    

};

#endif
