#ifndef WebSocketServer_HandDetector_h
#define WebSocketServer_HandDetector_h
#include <XnCppWrapper.h>


#include "WebSocket/Server.h"


class HandDetector {
    xn::Context& context_;
    xn::HandsGenerator hands_;
    xn::GestureGenerator gesture_;
    
    Server* server;
    std::string type;
    
    XnCallbackHandle gestureCallback,handsCallback,gestureChangeCallback;
    
    bool tracking;
    XnUserID trackingUserID;
    
public:
    explicit HandDetector(Server *s,xn::Context &context);
    ~HandDetector();
    
    static void XN_CALLBACK_TYPE GestureProgress(xn::GestureGenerator& gesture,const XnChar* strGesture,const XnPoint3D* pPosition, XnFloat fProgress, void* that);
    static void XN_CALLBACK_TYPE GestureRecognized(xn::GestureGenerator& gesture, const XnChar* strGesture,const XnPoint3D* pIDPosition, const XnPoint3D* pEndPosition, void* that);
    
    static void XN_CALLBACK_TYPE HandCreate(xn::HandsGenerator& hands,XnUserID nId,const XnPoint3D* pPosition, XnFloat fTime,void* that);
    static void XN_CALLBACK_TYPE HandUpdate(xn::HandsGenerator& hands,XnUserID nId,const XnPoint3D* pPosition,XnFloat fTime,void* that);
    static void XN_CALLBACK_TYPE HandDestroy(xn::HandsGenerator& hands,XnUserID nId, XnFloat fTime, void* that);

    void onGestureProgress(xn::GestureGenerator& gesture, const XnChar* strGesture, const XnPoint3D* Position,XnFloat progress);
    void onGestureRecognized(xn::GestureGenerator& gesture, const XnChar* strGesture,const XnPoint3D* pIDPosition, const XnPoint3D* pEndPosition);
    
    void onHandCreate(xn::HandsGenerator& hands,XnUserID nId,const XnPoint3D* pPosition, XnFloat fTime);
    void onHandUpdate(xn::HandsGenerator& hands,XnUserID nId,const XnPoint3D* pPosition,XnFloat fTime);
    void onHandDestroy(xn::HandsGenerator& hands,XnUserID nId, XnFloat fTime);

    void send(std::string action, XnUserID nId, const XnPoint3D *p);
};

#endif
