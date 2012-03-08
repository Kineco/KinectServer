#include "DepthMap.h"



DepthMap::DepthMap(xn::DepthGenerator &depth)
:depth_(depth){

    depth_.GetMetaData(depthMD);
    wid = depthMD.XRes();
    hei = depthMD.YRes();

    
}

Json::Value DepthMap::getDepth(const int x,const int y){
    printf("now we are sending depth----------------------->:\n");
    Json::Value root,kineco,coordinate,depth;
    

    boost::mutex::scoped_lock lock(mutex);
    int depth_value;
    depth_value = depthMD(x,y);
    if(depth_value==0)
        depth_value = 1000;
    
    //出力形式に整形
    depth["value"] =boost::lexical_cast<std::string>(depth_value);//ss_depth.str();
    kineco["coordinate"]["x"] = x;
    kineco["coordinate"]["y"] = y;
    kineco["Depth"] = depth;
    root["Kineco"] = kineco;
    
    return kineco;

}


void DepthMap::update(){

    boost::mutex::scoped_lock lock(mutex);
    depth_.GetMetaData(depthMD);


}

