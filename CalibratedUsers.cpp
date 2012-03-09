#include "CalibratedUsers.h"


CalibratedUsers::CalibratedUsers(xn::UserGenerator &user_)
:userGen_(user_){
    
    XnSkeletonJoint parts_joint[NUM_JOINTS] = {XN_SKEL_HEAD,XN_SKEL_TORSO,XN_SKEL_NECK,XN_SKEL_RIGHT_HIP,XN_SKEL_RIGHT_FOOT,XN_SKEL_RIGHT_KNEE,XN_SKEL_RIGHT_SHOULDER,XN_SKEL_RIGHT_ELBOW,XN_SKEL_RIGHT_HAND,XN_SKEL_LEFT_HIP,XN_SKEL_LEFT_FOOT,XN_SKEL_LEFT_KNEE,XN_SKEL_LEFT_SHOULDER,XN_SKEL_LEFT_ELBOW,XN_SKEL_LEFT_HAND};
    
    std::string parts_str[NUM_JOINTS] ={"head","torso","neck","right_hip","right_foot","right_knee","right_shoulder","right_elbow","right_hand","left_hip","left_foot","left_knee","left_shoulder","left_elbow","left_hand"} ;
    
    for(int i=0;i<NUM_JOINTS;i++){
        parts_.insert(std::map<XnSkeletonJoint,std::string>::value_type(parts_joint[i],parts_str[i]));
    }
    
    calibratedCount = 0;
}

void CalibratedUsers::update(){
    
    // 検出中のユーザ数とユーザID（非トラッキング含む）
    boost::mutex::scoped_lock lk(mutex);
    XnUInt16 numUsers = MAX_USER_NUM;
    XnUserID users[MAX_USER_NUM];
    userGen_.GetUsers(users,numUsers);
    calibratedCount = 0;
    for(int i=0;i<numUsers;i++)
    {
        XnUserID userID = users[i];
        /* ユーザがトラッキング中の場合は、骨格データの取得とキャリブレーション中ユーザ数、ユーザIDの保持*/
        if(userGen_.GetSkeletonCap().IsTracking(userID))
        {
            for(std::map<XnSkeletonJoint, std::string>::iterator it=parts_.begin(); it!=parts_.end();it++){
                XnSkeletonJointPosition jointPosition;
                
                userGen_.GetSkeletonCap().GetSkeletonJointPosition(userID, (*it).first, jointPosition);
                skeletons[userID][(*it).first] = jointPosition.position;
            }
            
            calibratedUsers[calibratedCount] = userID;
            calibratedCount++;
        }
        
    }

}

Json::Value CalibratedUsers::getSkeletonById(const int id_){
    
    XnUserID userID = MAX_USER_NUM + 1;
    boost::mutex::scoped_lock lk(mutex);
    for(int i=0;i<calibratedCount;i++)
    {
        if(calibratedUsers[i]==id_)
            userID = i;
    }
    if(userID > MAX_USER_NUM)
        throw std::runtime_error("User is not tracking");
    
    Json::Value root,config,kineco,user;
    user["id"] = id_;
    int count = 0;
    for (std::map<XnSkeletonJoint,std::string>::iterator it=parts_.begin();it!=parts_.end();it++) {
        XnVector3D position = skeletons[id_][(*it).first];
        user["eJoint"][count]["part"] = (*it).second;
        user["eJoint"][count]["cordinate"]["x"] = position.X;
        user["eJoint"][count]["cordinate"]["y"] = position.Y;
        user["eJoint"][count]["cordinate"]["z"] = position.Z;
	count++;
    }
    kineco["user"] = user;
    root["Kineco"] = kineco;
    return root;
    
}

Json::Value CalibratedUsers::getSkeletonJointPosition(const int id_, std::string ejoint_str){
    
    printf("now we are sending skeleton  ----------------------->:url\n");
   
    boost::mutex::scoped_lock lk(mutex);
    XnUserID userID = MAX_USER_NUM + 1;
    //引数のIDをチェック、トラッキング中のユーザでなければ例外を投げる。
    for(int i=0;i<calibratedCount;i++)
    {
        if(calibratedUsers[i]==id_)
            userID = i;
    }
    if(userID > MAX_USER_NUM)
        throw std::runtime_error("User is not tracking");
    
    //引数 ejointをチェック
    std::map<XnSkeletonJoint,std::string>::iterator it;
    bool joint_available = false;
    XnSkeletonJoint ejoint;
    for(it=parts_.begin();it!=parts_.end();it++)
    {
        if(!ejoint_str.compare((*it).second)){
            joint_available = true;
            ejoint = (*it).first;
            break;
        }
    }
    if(!joint_available)
        throw std::runtime_error("eJoint \""+ ejoint_str + "\" is not Available");
    
    //レスポンスJSONの作成
    Json::Value root,config,kineco,user;
    user["id"] = id_;

    XnVector3D position = skeletons[id_][ejoint];
    user["eJoint"]["part"] = (*it).second;
    user["eJoint"]["cordinate"]["x"] = position.X;
    user["eJoint"]["cordinate"]["y"] = position.Y;
    user["eJoint"]["cordinate"]["z"] = position.Z;
    
    kineco["user"] = user;
    root["Kineco"] = kineco;
    return root;
   
}


Json::Value CalibratedUsers::getNumberOfCalibratedUsers(){
    
    
    Json::Value root,kineco,config,user;
    printf("now we are sending numberOfCalibratedUsers  ----------------------->:url\n");     
    boost::mutex::scoped_lock lk(mutex);
    user["calibratedUsersCount"]=calibratedCount;
    kineco["user"] = user;
    root["Kineco"] = kineco;
    return root;
    
}




Json::Value CalibratedUsers::getCalibratedUserIds(){
    
    Json::Value kineco,root,user;
    printf("now we are sending calibratedUserIds----------------------->:url\n");
    if(calibratedCount!=0){
        boost::mutex::scoped_lock lk(mutex);
        for(int count =0;count<calibratedCount;count++){ 
            user[count]["id"] = calibratedUsers[count];
        }
    }else {
        user["id"] = "";
    }
    
    kineco["user"] = user;
    root["Kineco"] = kineco;
    return root;
}






