#include <iostream>

#include "AbstractPoseDetector.h"

AbstractPoseDetector::AbstractPoseDetector(Server *s, xn::Context &context)
:AbstractUserDetector(context)
{
    server = s;
    type_ = "pose";
    ver_ = "1.0";
    posingTime = 0;
    posing = false;
    lostTime = 0;
    
}

Vector3D AbstractPoseDetector::getVector(XnSkeletonJoint joint1, XnSkeletonJoint joint2)
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
void AbstractPoseDetector::detectPose()
{
    std::stringstream output;
    
    output << "{\"kineco\": " << std::endl;
    output <<   "{\"config\": ";
    output <<       "{\"version\": " << ver_ << "," << std::endl;
    output <<       " \"type\": \"" << type_ << "\"}," << std::endl;
    output <<   " \"pose\": " << std::endl;
    output <<       "{\"name\": \"" << poseName << "\"," << std::endl;
    output <<       " \"action\": \"detect\" ," << std::endl;
    output <<       " \"id\": \"" << tracking_user << "\"}" << std::endl; 
    output <<   "}" << std::endl;
    output << "}" << std::endl;
    
    server->broadcastText(output.str());
    
}
void AbstractPoseDetector::lostPose()
{
    std::stringstream output;
    
    output << "{\"kineco\": " << std::endl;
    output <<   "{\"config\": ";
    output <<       "{\"version\": " << ver_ << "," << std::endl;
    output <<       " \"type\": \"" << type_ << "\"}," << std::endl;
    output <<   " \"pose\": " << std::endl;
    output <<       "{\"name\": \"" << poseName << "\"," << std::endl;
    output <<       " \"action\": \"lost\" ," << std::endl;
    output <<       " \"id\": \"" << tracking_user << "\"}" << std::endl; 
    output <<   "}" << std::endl;
    output << "}" << std::endl;
    
    server->broadcastText(output.str());
}
