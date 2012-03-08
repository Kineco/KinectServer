#include "DetectedUsers.h"

DetectedUsers::DetectedUsers(xn::UserGenerator &user_)
:userGen_(user_){
    detectedUsersCount =0;
    for (int i=0; i<MAX_USERS_NUM; i++) {
        detectedUsers[i] = 0;
    }
    
}


Json::Value DetectedUsers::getUserIds(){
    
    
    Json::Value root,kineco,config,user;
    printf("now we are sending detectedUserIds  ----------------------->:url\n");
    
    if(detectedUsersCount!=0){
        for (int i=0;i<detectedUsersCount;i++){
            user[i]["id"] = detectedUsers[i];	
        }
        
    }
    kineco["user"] = user;
    root["Kineco"] = kineco;
    
    return root;
    
    
}

Json::Value DetectedUsers::getNumberOfDetectedUsers(){
    
    printf("now we are sending numberOfDetectedUsers  ----------------------->:url\n"); 
    Json::Value user,kineco,root;
    mutex.lock();
    user["DetectedUsersCount"] = detectedUsersCount;
    mutex.unlock();
    
    kineco["user"] = user;
    root["Kineco"] = kineco;
    
    return root;
    
}


Json::Value DetectedUsers::getCenterPoints(){
    Json::Value root,kineco,user;
    
    printf("now we are sending centerPoints  ----------------------->:url\n");
    mutex.lock();
    if(detectedUsersCount!=0){
        for (int i=0;i<detectedUsersCount;i++){
            user[i]["id"] = detectedUsers[i];
            user[i]["coordinate"]["x"]  = boost::lexical_cast<std::string>(centerPoint[i].X);     
            user[i]["coordinate"]["y"]  = boost::lexical_cast<std::string>(centerPoint[i].Y); 
        	user[i]["coordinate"]["z"]  = boost::lexical_cast<std::string>(centerPoint[i].Z);
        }
    }
    mutex.unlock();
    
    kineco["user"] = user;
    root["Kineco"] = kineco;
    
    return root;
    
}



Json::Value DetectedUsers::getDepthById(const int id){
    
    printf("now we are sending depth by id  ----------------------->:url\n");
    Json::Value root,kineco,user;
    if(detectedUsersCount!=0){
        int i;
        int user_num = 0;
        mutex.lock();
        for(i=0;i<detectedUsersCount;i++){
            if(id==detectedUsers[i]) {
                user_num = i;
                break;
            }
        }
        
        if(user_num == 0)
            throw std::runtime_error(" user is not detected  ");
        
       	user["id"] = id;
        user["depth"]  = boost::lexical_cast<std::string>(centerPoint[user_num].Z);
        mutex.unlock();
    }
    kineco["user"] = user;
    root["Kineco"] = kineco;
    
    return root;
}



Json::Value DetectedUsers::getCenterPointById(const int id){
    
    
    printf("now we are sending centerPoint by id  ----------------------->:url\n");
    Json::Value root,kineco,user;
    if(detectedUsersCount!=0){
        int i;
        int user_num = 0;
        mutex.lock();
        for(i=0;i<detectedUsersCount;i++){
            if(id==detectedUsers[i]) {
                user_num = i;
                break;
            }
        }
        
        if(user_num == 0)
            throw std::runtime_error(" user is not detected  ");
        
       	user["id"] = id;
        user["coordinate"]["x"]  = boost::lexical_cast<std::string>(centerPoint[i].X);     
        user["coordinate"]["y"]  = boost::lexical_cast<std::string>(centerPoint[i].Y); 
        user["coordinate"]["z"]  = boost::lexical_cast<std::string>(centerPoint[i].Z);
        mutex.unlock();
    }
    kineco["user"] = user;
    root["Kineco"] = kineco;
    
    return root;
    
}



void DetectedUsers::update(){
    
    XnUInt16 nUsers = MAX_USERS_NUM;
    mutex.lock();
    userGen_.GetUsers(detectedUsers, nUsers);
    detectedUsersCount = nUsers;

    for (int i = 0; i < nUsers; ++i){
        
        XnPoint3D com;
        userGen_.GetCoM(detectedUsers[i],com);
        centerPoint[i] = com;
        
    }
    mutex.unlock();
    
                          
    
}
