#include <iostream>
#include <sstream>

#include "PoseDetector.h"

#include "config.h"


#define E_ 0.3 //ポーズ計算時　角度の誤差
#define DT 5  //ポーズの持続時間

PoseDetector::PoseDetector(xn::Context &context, Server *s)
:AbstractUserDetector(context)
{
    posingTime = 0;
    lostTime = 0;
    posing = false;
    poseName = "stop";
    prevPoseName = "none";
    type_ = "pose";
    ver_ = 1.0;
    server = s;
}
PoseDetector::~PoseDetector()
{
    
}

void PoseDetector::detect()
{
    if(tracking){
        calcPose();
        KeepTime();
    }
}

bool PoseDetector::calcPose()
{
    try {
        Vector3D l_arm1, l_arm2;
        Vector3D r_arm1, r_arm2;
        
        Vector3D X_,Y_;
        
        //左手のベクトル
        l_arm1 = getVector(XN_SKEL_LEFT_SHOULDER,XN_SKEL_LEFT_ELBOW);
        l_arm2 = getVector(XN_SKEL_LEFT_ELBOW, XN_SKEL_LEFT_HAND);
        //右手のベクトル
        r_arm1 = getVector(XN_SKEL_RIGHT_SHOULDER, XN_SKEL_RIGHT_ELBOW);
        r_arm2 = getVector(XN_SKEL_RIGHT_ELBOW, XN_SKEL_RIGHT_HAND);
        
        //Y軸として首-頭　X軸として右肩-左肩
        X_ = getVector(XN_SKEL_LEFT_SHOULDER, XN_SKEL_RIGHT_SHOULDER);
        Y_ = getVector(XN_SKEL_NECK, XN_SKEL_HEAD);
        
        Vector3D l_arm,r_arm; //左右　肩-肘 肘-手　の平均ベクトル
        
        l_arm = (l_arm1 + l_arm2) / 2.0;
        r_arm = (r_arm1 + r_arm2) / 2.0;
        
        
        float l_cos_xz, l_cos_yz;
        // x-左腕平均 の角度
        l_cos_xz = (X_ * l_arm) / (X_.size() * l_arm.size());
        // y-左腕平均　の角度
        l_cos_yz = (Y_ * l_arm) / (Y_.size() * l_arm.size());
        
        float r_cos_xz, r_cos_yz;
        //x-右腕平均の角度
        r_cos_xz = (X_ * r_arm) / (X_.size() * r_arm.size());
        //y-右腕平均の角度
        r_cos_yz = (Y_ * r_arm) / (Y_.size() * r_arm.size());
        
        float l_arm_xz, l_arm_yz;
        //x-左肘から手 の角度
        l_arm_xz = (X_ * l_arm2) / (X_.size() * l_arm2.size());
        //y-左肘から手 の角度
        l_arm_yz = (Y_ * l_arm2) / (Y_.size() * l_arm2.size());
        
        float r_arm_xz, r_arm_yz;
        
        r_arm_xz = (X_ * r_arm2) / (Y_.size() * r_arm2.size());
        r_arm_yz = (Y_ * r_arm2) / (Y_.size() * r_arm2.size());
        
        bool front,back,left,right;
        
        
        
        front = (fabsf(r_cos_xz)< E_ &&  fabsf(l_cos_xz) < E_ && fabsf(r_cos_yz) < E_ && fabsf(l_cos_yz) < E_);
        
        back  = (fabsf(r_cos_xz) <E_ &&  fabsf(l_cos_xz) < E_ && (1.0-r_cos_yz) < E_ && (1.0 - l_cos_yz) < E_);
        
        left  = ((1.0+r_cos_xz) < E_ &&  (1.0+l_arm_xz) < E_ && fabsf(r_cos_yz) < E_ && fabsf(l_arm_yz)  < E_);
        
        right = ((1.0-r_arm_xz) < E_ &&  (1.0-l_cos_xz) < E_  && fabsf(r_arm_yz) < E_ && fabsf(l_cos_yz) < E_);
        
        
        
        if (!(front || back || left || right)){
            //Poseが検出されなかった場合の処理    
            if (posing) {
                //Poseが検出されている状態だった場合はPoseが終了したことを通知
                prevPoseName = poseName;
                posingTime = 0;
                lostPose(poseName);
                
            }
            posing = false;
            poseName = "stop";
            
        }else {
            
            prevPoseName = poseName;
            if(front){
                poseName = "front";
            }else if(back){
                poseName = "back";
            }else if(right){
                poseName = "right";
            }else if(left){
                poseName = "left";
            }
        }
        
    }catch(std::exception ex){
        return false;
    }
    
    return true;
}
/*
 *　Poseの持続時間を計測
 */
void PoseDetector::KeepTime()
{
    std::cout << poseName << " time " << posingTime << std::endl; 
    if(prevPoseName==poseName){
        //同じPoseが継続中
        posingTime++;
        if(posingTime==DT)
        {
            detectPose(poseName);
            posing = true;
        }
        
    }else {
        //新しいPose検出
        posingTime=0;
    }
}
void PoseDetector::detectPose(std::string pose)
{
    std::stringstream output;
 
    output << "{\"kineco\": " << std::endl;
    output <<   "{\"config\": ";
    output <<       "{\"version\": " << ver_ << "," << std::endl;
    output <<       " \"type\": \"" << type_ << "\"}," << std::endl;
    output <<   " \"pose\": " << std::endl;
    output <<       "{\"name\": \"" << pose << "\"," << std::endl;
    output <<       " \"action\": \"detect\" ," << std::endl;
    output <<       " \"id\": \"" << tracking_user << "\"}" << std::endl; 
    output <<   "}" << std::endl;
    output << "}" << std::endl;
    
    std::cout << output.str() ;
    server->broadcastText(output.str());
}
void PoseDetector::lostPose(std::string pose)
{
    std::stringstream output;
    
    output << "{\"kineco\": " << std::endl;
    output <<   "{\"config\": ";
    output <<       "{\"version\": " << ver_ << "," << std::endl;
    output <<       " \"type\": \"" << type_ << "\"}," << std::endl;
    output <<   " \"pose\": " << std::endl;
    output <<       "{\"name\": \"" << pose << "\"," << std::endl;
    output <<       " \"action\": \"lost\" ," << std::endl;
    output <<       " \"id\": \"" << tracking_user << "\"}" << std::endl; 
    output <<   "}" << std::endl;
    output << "}" << std::endl;
    
    server->broadcastText(output.str());
}
Vector3D PoseDetector::getVector(XnSkeletonJoint joint1, XnSkeletonJoint joint2)
{
    XnSkeletonJointPosition pos1, pos2;
    
    user_gen.GetSkeletonCap().GetSkeletonJointPosition(tracking_user,joint1,pos1);
    user_gen.GetSkeletonCap().GetSkeletonJointPosition(tracking_user,joint2,pos2);
    
    if (pos1.fConfidence < 0.5 || pos2.fConfidence < 0.5) {
        throw std::runtime_error("confidence");
    }
    XnPoint3D p[2];
    p[0] = pos1.position;
    p[1] = pos2.position;
    
    Vector3D output;
    output.X = p[0].X - p[1].X;
    output.Y = p[0].Y - p[1].Y;
    output.Z = p[0].Z - p[1].Z;
    
    return output;
}
