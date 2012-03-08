#ifndef WebSocketServer_AbstractUserDetector_h
#define WebSocketServer_AbstractUserDetector_h
#include <XnCppWrapper.h>
#include <boost/thread.hpp>


class AbstractUserDetector {
protected:
    xn::Context &context_;
    xn::UserGenerator user_gen;
    XnCallbackHandle user_handle;
    XnCallbackHandle calibration_handle;
    XnCallbackHandle pose_handle;
    XnChar *pose;
    XnUserID tracking_user;
    bool tracking;

    
    boost::mutex mutex;
    typedef boost::mutex::scoped_lock lock;
public:
    explicit AbstractUserDetector(xn::Context &context);
    ~AbstractUserDetector();
    
    static void XN_CALLBACK_TYPE s_NewUser(xn::UserGenerator& userGen, XnUserID userID, void* that);
    static void XN_CALLBACK_TYPE s_LostUser(xn::UserGenerator& userGen, XnUserID userID, void* that);
    
    static void XN_CALLBACK_TYPE s_CalibrationStart(xn::SkeletonCapability& cap, XnUserID userID, void* that);
    static void XN_CALLBACK_TYPE s_CalibrationEnd(xn::SkeletonCapability& cap, XnUserID userID, XnBool success, void* that);
    
    static void XN_CALLBACK_TYPE s_PoseStart(xn::PoseDetectionCapability& cap, const XnChar* pose, XnUserID userID, void* that);
    static void XN_CALLBACK_TYPE s_PoseEnd(xn::PoseDetectionCapability& cap, const XnChar* pose, XnUserID userId, void* that);
    
    bool isTracking();
    XnUserID trackingUserID();
    
    
    void startCalibration(XnUserID userID);
    
    virtual void onNewUser(xn::UserGenerator& userGen, XnUserID userID);
    virtual void onLostUser(xn::UserGenerator& userGen, XnUserID userID);
    
    virtual void onCalibrationStart(xn::SkeletonCapability& cap, XnUserID userID);
    virtual void onCalibrationEnd(xn::SkeletonCapability& cap, XnUserID userID,XnBool success);
    
    virtual void onPoseStart(xn::PoseDetectionCapability& cap, const XnChar* pose, XnUserID userID);
    virtual void onPoseEnd(xn::PoseDetectionCapability& cap, const XnChar* pose, XnUserID userID);
    
};






#endif
