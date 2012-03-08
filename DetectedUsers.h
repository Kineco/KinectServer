#ifndef DetectedUsers_h
#define DetectedUsers_h
#include <iostream>
#include <XnCppWrapper.h>

#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

#include "DepthParam.h"
#include "json/json.h"

#define MAX_USERS_NUM 15


class DetectedUsers
{
  public: 
    DetectedUsers(xn::UserGenerator &user_);
    Json::Value getUserIds();
    Json::Value getNumberOfDetectedUsers();
    Json::Value getDepthById(const int id);
    Json::Value getCenterPointById(const int id);
    Json::Value getCenterPoints();
    void update();
       

private:
    xn::UserGenerator userGen_;
    
    XnUserID detectedUsers[MAX_USERS_NUM];
    XnPoint3D centerPoint[MAX_USERS_NUM];
    int detectedUsersCount;
    boost::mutex mutex;
    

};
#endif

