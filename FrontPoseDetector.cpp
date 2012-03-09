#include <iostream>
#include "FrontPoseDetector.h"

#define E_ 0.3 //ポーズ計算時　角度の誤差
#define DT 5  //ポーズの持続時間

FrontPoseDetector::FrontPoseDetector(Server *s, xn::Context &context)
:AbstractPoseDetector(s,context)
{
    poseName = "front";
}

void FrontPoseDetector::detect()
{
    //トラッキング中ならポーズの計算を行う。
    if(tracking){
        
        //ポーズ中ならposingTimeを追加
        if(calcPose()){
            posingTime++;
            
            //ポーズが規定以上続いたらポーズ検出
            if(posingTime > DT && !posing){
                posing = true;
                detectPose();
            }
        //ポーズ中でない場合
        } else {
            //ポーズ中からポーズ無しに移行 ポーズ消失
            if(posing){
                posingTime=0; 
                posing = false;
                lostPose();
            }
        }
    }
}

bool FrontPoseDetector::calcPose()
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
        
        return (fabsf(r_cos_xz)< E_ &&  fabsf(l_cos_xz) < E_ && fabsf(r_cos_yz) < E_ && fabsf(l_cos_yz) < E_);
        
    }catch(std::exception ex){
        return false;
    }
}


