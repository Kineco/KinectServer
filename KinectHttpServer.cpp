#include "KinectHttpServer.h"


#define POSE_TO_USE "Psi"
#define XN_CALIBRATION_FILE_NAME "UserCalibration.bin"

void KinectHttpServer::XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& generator,XnUserID nId, void* pCookie){
    
    KinectHttpServer *server = static_cast<KinectHttpServer*>(pCookie);
    if ( !(server->LoadCalibration( generator )) ) {
        
        if (server->pose[0] != '\0') {
            generator.GetPoseDetectionCap().StartPoseDetection(server->pose, nId);
        }
        else {
            generator.GetSkeletonCap().RequestCalibration(nId, TRUE);
        }
    }
    
}

void KinectHttpServer::XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& generator, XnUserID nId,void* pCookie){
    
    printf("Lost User: %d\n", nId);
    
}


void KinectHttpServer::XN_CALLBACK_TYPE Pose_Detected(xn::PoseDetectionCapability& pose, const XnChar* strPose,XnUserID nId, void* pCookie){
    KinectHttpServer *server = static_cast<KinectHttpServer*>(pCookie);
    
	printf("Pose %s for user %d\n", strPose, nId);
	server->user.GetPoseDetectionCap().StopPoseDetection(nId);
	server->user.GetSkeletonCap().RequestCalibration(nId, TRUE);
}


void KinectHttpServer::XN_CALLBACK_TYPE Calibration_Start(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie){
    printf("Starting calibration for user %d\n", nId);
}


void  KinectHttpServer::XN_CALLBACK_TYPE Calibration_End(xn::SkeletonCapability& capability, XnUserID nId,XnBool bSuccess, void* pCookie){
    KinectHttpServer *server = static_cast<KinectHttpServer *>(pCookie);
    xn::UserGenerator user_ = server->user;
    if (bSuccess){
        printf("User calibrated\n");
        server->SaveCalibration(user_);        
        user_.GetSkeletonCap().StartTracking(nId);
    }else{
        printf("Failed to calibrate user %d\n", nId);
        user_.GetPoseDetectionCap().StartPoseDetection(POSE_TO_USE,nId);
    }
}


void KinectHttpServer::SaveCalibration( xn::UserGenerator& userGenerator )
{
    printf("SaveCalibration\n");
    
    XnUserID aUserIDs[20] = {0};
    XnUInt16 nUsers = 20;
    userGenerator.GetUsers( aUserIDs, nUsers );
    for ( int i = 0; i < nUsers; ++i ) {
        if ( userGenerator.GetSkeletonCap().IsCalibrated(aUserIDs[i]) ) {
            printf("Save user's calibration to file: user %d\n", aUserIDs[i]);
            userGenerator.GetSkeletonCap().SaveCalibrationDataToFile(aUserIDs[i], XN_CALIBRATION_FILE_NAME);
            break;
        }
    }
}

// Load calibration from file
bool KinectHttpServer::LoadCalibration( xn::UserGenerator& userGenerator )
{
    printf("LoadCalibration\n");
    
    XnUserID aUserIDs[20] = {0};
    XnUInt16 nUsers = 20;
    userGenerator.GetUsers(aUserIDs, nUsers);
    for (int i = 0; i < nUsers; ++i) {
        if (userGenerator.GetSkeletonCap().IsCalibrated(aUserIDs[i]))
            continue;
        
        if (userGenerator.GetSkeletonCap().IsCalibrating(aUserIDs[i]))
            continue;
        
        printf("Load user's calibration from file: user %d\n", aUserIDs[i]);
        XnStatus rc = userGenerator.GetSkeletonCap().LoadCalibrationDataFromFile(aUserIDs[i], XN_CALIBRATION_FILE_NAME);
        if (rc == XN_STATUS_OK) {
            printf("Make sure state is coherent: user %d\n", aUserIDs[i]);
            userGenerator.GetPoseDetectionCap().StopPoseDetection(aUserIDs[i]);
            userGenerator.GetSkeletonCap().StartTracking(aUserIDs[i]);
            return true;
        }
        
        break;
    }
    
    return false;
}




/* アクセサメソッド　ここから */
DepthMap* KinectHttpServer::depthMap(){ return depthMap_; }
RgbImage* KinectHttpServer::rgbImage(){ return rgbImage_; }
DepthImage *KinectHttpServer::depthImage(){ return depthImage_; }
PointCloud *KinectHttpServer::pointCloud(){ return pointCloud_; }
CalibratedUsers *KinectHttpServer::calibratedUsers(){ return calibratedUsers_; }
DetectedUsers *KinectHttpServer::detectedUsers(){ return detectedUsers_; }
/* アクセサメソッド　ここまで */

KinectHttpServer::KinectHttpServer(int port_,int ws_port, xn::Context context_)
:context(context_)
{
    //Kinect 初期化 ImageGenerator DepthGenerator UserGenerator
    XnStatus rc;
    rc = context.FindExistingNode(XN_NODE_TYPE_IMAGE, image);
    if(rc != XN_STATUS_OK){
        throw std::runtime_error(xnGetStatusString(rc));
    }
    
    rc = context.FindExistingNode(XN_NODE_TYPE_DEPTH, depth);
    if(rc != XN_STATUS_OK){
        throw std::runtime_error(xnGetStatusString(rc));
    }
    
    rc = user.Create(context);
    if(rc != XN_STATUS_OK){
        throw std::runtime_error(xnGetStatusString(rc));
    }
    
    xn::AlternativeViewPointCapability viewPoint = depth.GetAlternativeViewPointCap();
    viewPoint.SetViewPoint(image);
    
    
    // コールバック関数の設定
    user.RegisterUserCallbacks(&KinectHttpServer::User_NewUser, &KinectHttpServer::User_LostUser,this, user_handle);
    user.GetPoseDetectionCap().RegisterToPoseCallbacks(&KinectHttpServer::Pose_Detected, NULL, this, pose_handle);
    if(user.GetSkeletonCap().NeedPoseForCalibration())
    {
        rc = user.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION);
        
        pose = new XnChar[20];
        user.GetSkeletonCap().GetCalibrationPose(pose);
        
        user.GetSkeletonCap().RegisterCalibrationCallbacks(&KinectHttpServer::Calibration_Start, &KinectHttpServer::Calibration_End, this, calibration_handle);
        
    }else {
        pose = NULL;
        pose_handle = NULL;
    }
    
    // Set the profile
    user.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
    
    
    
    rgbImage_ = new RgbImage(image);
    depthMap_ = new DepthMap(depth);
    depthImage_ = new DepthImage(depth);
    pointCloud_ = new PointCloud(depth);
    calibratedUsers_ = new CalibratedUsers(user);
    detectedUsers_ = new DetectedUsers(user);
    
    context.StartGeneratingAll();
    
    
    
    port = port_;
    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, port, NULL, NULL, &RequestHandler::handle_request, (void *)this,MHD_OPTION_END);   
    
    
    if(daemon == NULL)
        throw std::runtime_error("error: create HTTP Daemon");  
    
    wsServer = new Server(ws_port);
    wsServer->run();
    userDetector = new UserDetector(wsServer,context);
    handDetector = new HandDetector(wsServer,context);
    
    
}


void KinectHttpServer::updateData(){
    
    depthMap_->update();
    rgbImage_->update();
    depthImage_->update();
    pointCloud_->update();
    calibratedUsers_->update();
    detectedUsers_->update();
}



