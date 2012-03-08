#include "PointCloud.h"
#define Ver_ 1.0

PointCloud::PointCloud(xn::DepthGenerator& depth)
:depth_(depth)
{    

    depth.GetMetaData(depthMD);
    wid = depthMD.XRes();
    hei = depthMD.YRes();
}


void PointCloud::update(){
    
    boost::mutex::scoped_lock lk(mutex);
    depthParam.setParam(depth_);
    depthParam.setParam(depthMD);
    depth_.GetMetaData(depthMD);

    
    
}

Json::Value PointCloud::getPointCloud(){
    
    int index;
    XnPoint3D *buffer = new XnPoint3D[wid * hei * sizeof(XnPoint3D)];
    mutex.lock();
    depthParam.setParam(depth_);
    depthParam.setParam(depthMD);
    depthParam.depth2point(depthMD.Data(), buffer);
    mutex.unlock();

    printf("now we are sending pointCloud\n");   
    Json::Value pointCloud,kineco,config,root;
  
    for(int y =0;y<hei;y++){
        for(int x=0;x<wid;x++){
            index = y * wid + x;

            pointCloud[index]["x"]  = boost::lexical_cast<std::string>(buffer[index].X);
            pointCloud[index]["y"]  = boost::lexical_cast<std::string>(buffer[index].Y); 
            pointCloud[index]["z"]  = boost::lexical_cast<std::string>(buffer[index].Z);
            
        }
    }        
    
    config["version"] = Ver_;
    kineco["config"] = config;
    kineco["point_Cloud"] = pointCloud;
    root["Kineco"] = kineco;
    
    return root;
    
}
