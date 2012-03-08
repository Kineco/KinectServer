#include <iostream>
#include <stdexcept>
#include <sstream>

#include "config.h"
#include "HandDetector.h"



void XN_CALLBACK_TYPE HandDetector::GestureProgress(xn::GestureGenerator& gesture, const XnChar* strGesture, const XnPoint3D* pPosition,XnFloat fProgress, void* that)
{
    static_cast<HandDetector*>(that)->onGestureProgress(gesture,strGesture,pPosition,fProgress);
}
void XN_CALLBACK_TYPE HandDetector::GestureRecognized(xn::GestureGenerator& gesture,const XnChar* strGesture,const XnPoint3D* pIDPosition, const XnPoint3D* pEndPosition,void* that)
{
    static_cast<HandDetector*>(that)->onGestureRecognized(gesture,strGesture,pIDPosition,pEndPosition);
}

void XN_CALLBACK_TYPE HandDetector::HandCreate(xn::HandsGenerator& hands,XnUserID nId,const XnPoint3D* pPosition,XnFloat fTime,void* that)
{
    static_cast<HandDetector*>(that)->onHandCreate(hands,nId,pPosition,fTime);
}
void XN_CALLBACK_TYPE HandDetector::HandUpdate(xn::HandsGenerator& hands, XnUserID nId, const XnPoint3D* pPosition,XnFloat fTime, void* that)
{
    static_cast<HandDetector*>(that)->onHandUpdate(hands,nId,pPosition,fTime);
}
void XN_CALLBACK_TYPE HandDetector::HandDestroy(xn::HandsGenerator& hands,XnUserID nId,XnFloat fTime,void* that)
{
    static_cast<HandDetector*>(that)->onHandDestroy(hands,nId,fTime);
}


HandDetector::HandDetector(Server *s, xn::Context &context)
:context_(context)
{
    tracking = false;
    trackingUserID = 0;
    type = "hands";
    server = s;
    
    XnStatus rc;
    
    rc = context_.FindExistingNode(XN_NODE_TYPE_HANDS, hands_);
    
    hands_.Create(context_);
    
    rc = context_.FindExistingNode(XN_NODE_TYPE_GESTURE, gesture_);
    
    gesture_.Create(context_);
    
    gesture_.AddGesture("Click", 0);
    
    gesture_.RegisterGestureCallbacks(&HandDetector::GestureRecognized, &HandDetector::GestureProgress, this, gestureCallback);
    
    hands_.RegisterHandCallbacks(&HandDetector::HandCreate, &HandDetector::HandUpdate, &HandDetector::HandDestroy, this, handsCallback);
}

HandDetector::~HandDetector()
{
    
}

void HandDetector::onGestureProgress(xn::GestureGenerator &gesture, const XnChar *strGesture, const XnPoint3D *Position, XnFloat progress)
{
    std::cout << strGesture << std::endl;
}

void HandDetector::onGestureRecognized(xn::GestureGenerator &gesture, const XnChar *strGesture, const XnPoint3D *pIDPosition, const XnPoint3D *pEndPosition)
{
    //std::cout << "GestureRecognized" << std::endl;
    hands_.StartTracking(*pEndPosition);
}

void HandDetector::onHandCreate(xn::HandsGenerator &hands, XnUserID nId, const XnPoint3D *pPosition, XnFloat fTime)
{
    tracking = true;
    trackingUserID = nId;
    
    send("create",nId,pPosition);
}

void HandDetector::onHandUpdate(xn::HandsGenerator &hands, XnUserID nId, const XnPoint3D *pPosition, XnFloat fTime)
{
    send("update",nId,pPosition);
}

void HandDetector::onHandDestroy(xn::HandsGenerator &hands, XnUserID nId, XnFloat fTime)
{
    if(tracking)
    {
       // std::cout << "HandDestroy" << std::endl;
        tracking = false;
        send("destroy",nId,NULL);
        
    }
}

void HandDetector::send(std::string action, XnUserID nId, const XnPoint3D *p)
{
    std::stringstream output;
    
    output << "{\"kineco\": {" << std::endl;
    output << "\"version\": 1.0 ," << std::endl;
    output <<       "\"type\": \"" + type + "\" ," << std::endl;
    output <<       "\"action\": \"" + action + "\" ," << std::endl;
    if(action!="destroy"){
        output <<       "\"position\": " << std::endl;
        output << "{\"x\": " <<  p->X << ", \"y\": " << p->Y << ", \"z\": " << p->Z << "} ," << std::endl;
    }
    output << "\"user\": " << nId << "} }" << std::endl;
    
   // std::cout << output.str().size() << std::endl;
    server->broadcastText(output.str());
    
}
