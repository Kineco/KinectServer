#ifndef KinectHttpServer_h
#define KinectHttpServer_h
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <XnCppWrapper.h>

#include <microhttpd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <json/json.h>

#include <time.h>


#include "DepthMap.h"
#include "RgbImage.h"
#include "DepthImage.h"
#include "PointCloud.h"
#include "CalibratedUsers.h"
#include "DetectedUsers.h"
#include "RequestHandler.h"

#include "Websocket/Server.h"
#include "HandDetector.h"
#include "UserDetector.h"


typedef std::vector<float> depth_hist;


class KinectHttpServer
{

public:
    explicit KinectHttpServer(int port_,int ws_port,xn::Context context_);
    /*  request_handler 
     *   クライアントからのリクエストを受け、レスポンスを作成する。
     */
    static int request_handler(void *cls, struct MHD_Connection *connection,
								 const char *url, const char *method,
								 const char *version, const char *upload_data,
								 size_t *upload_data_size, void **con_cls);
    
    /*  User_NewUser, User_LostUser
     *  Pose_Detected, Calibration_Start, Calibration_End
     *
     *  ユーザ関係のコールバック関数群。
     *  コールバックとして登録するため、クラス関数として定義。pCookieにはKinectHTTPServerのインスタンスが渡される。
     */
    static void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& generator,XnUserID nId, void* pCookie);
    static void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& generator, XnUserID nId,void* pCookie);
    static void XN_CALLBACK_TYPE Pose_Detected(xn::PoseDetectionCapability& pose, const XnChar* strPose,XnUserID nId, void* pCookie);
    static void XN_CALLBACK_TYPE Calibration_Start(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie);
    static void XN_CALLBACK_TYPE Calibration_End(xn::SkeletonCapability& capability, XnUserID nId,XnBool bSuccess, void* pCookie);    
    
    /* SaveCalibration, LoadCalibration 
     *  ユーザがロストした際にキャリブレーションの情報をファイルに保存、再びユーザを検出した際に
     * ファイルから読み込むことで、キャリブレーションが要らなくなる。
     */
    void SaveCalibration( xn::UserGenerator& userGenerator );
    bool LoadCalibration( xn::UserGenerator& userGenerator );
    
    
    /* updateData
     *  Kinectの情報が更新されたときに、各インスタンスの情報を更新する。
     *
     */
    void updateData();
    
    /*アクセサメソッド*/
    DepthMap *depthMap();
    RgbImage *rgbImage();
    DepthImage *depthImage();
    PointCloud *pointCloud();
    CalibratedUsers *calibratedUsers();
    DetectedUsers *detectedUsers();
    
    
private:

    //データを保持するインスタンス
    DepthMap *depthMap_;
    RgbImage *rgbImage_;
    DepthImage *depthImage_;
    PointCloud *pointCloud_;
    CalibratedUsers *calibratedUsers_;
    DetectedUsers *detectedUsers_;
    
    //WebSocketサーバおよび、関連するインスタンス
    Server *wsServer;
    UserDetector *userDetector;
    HandDetector *handDetector;
    
    
    struct MHD_Daemon *daemon;
    int port;
    
    //OpenNI関連のインスタンス
    xn::Context context;
    xn::ImageGenerator image;
    xn::DepthGenerator depth;
    xn::UserGenerator user;
    
    //UserGenerator CallbackHandle
    XnCallbackHandle user_handle;
    XnCallbackHandle calibration_handle;
    XnCallbackHandle pose_handle;
    XnChar *pose;
    

};
#endif
