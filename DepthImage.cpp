#include "DepthImage.h"


DepthImage::DepthImage(xn::DepthGenerator &depth)
:depth_(depth){
    depth_.GetMetaData(depthMD);
    
    //サイズ取得
    wid = depthMD.XRes();
    hei = depthMD.YRes();
}

Json::Value DepthImage::getDepthImageData(){
    
    printf("now we are sending depthImage data ----------------------->:\n");
    
    //
    int input_size = wid * hei * sizeof(XnUInt16);
    unsigned char *src = new unsigned char[input_size];
    unsigned char *out = new unsigned char[input_size * 2];
    mutex.lock();
    //メモリのコピー
    for(int j=0; j< hei; j++){
        for(int i=0;i<wid;i++){
            //16ビットのDepth値をunsigned charのバッファにコピー
            int index = ((j * wid) + i) * 2;
            src[index  ] = (0x00ff & depthMD(i, j));
            src[index+1] = (0xff00 & depthMD(i, j));
        }
    }
    mutex.unlock();
    size_t output_size = input_size * 2;
    int te = base64_encode(out, &output_size,src, input_size);
    std::cout << te << std::endl;
    out[output_size] = '\0';
    Json::Value root,config,kineco;
    config["width"] = wid;
    config["height"] = hei;
    std::cout << output_size << std::endl;
    kineco["pixels_DepthImage"] = std::string(reinterpret_cast<const char*>(out));
    kineco["config"] = config;
    root["kineco"] = kineco;
    delete src;
    return root;
}


std::vector<unsigned char> DepthImage::getDepthImage(){
    
    cv::Mat img;
    img = cv::Mat::zeros(cvSize(wid,hei), CV_8UC3);
    mutex.lock();
    depth_hist depthHist = getDepthHistgram();
    for (int j=0; j<hei; j++) {
        for (int i=0; i<wid; i++) {
            int index = ((j * wid ) + i )* 3;
            img.data[index  ] = depthHist[depthMD(i,j)];
            img.data[index+1] = depthHist[depthMD(i,j)];
            img.data[index+2] = 0;
        }
    }
    mutex.unlock();
    
    std::vector<unsigned char> buff;
    
    std::vector<int> param = std::vector<int>(2);
    param[0] = CV_IMWRITE_JPEG_QUALITY;
    param[1] = 95;
    cv::imencode(".jpg",img,buff,param);
    
    return buff;

    
}

void DepthImage::update(){
    boost::mutex::scoped_lock lk(mutex);
    depth_.GetMetaData(depthMD);
}

depth_hist DepthImage::getDepthHistgram(){
    
    const int MAX_DEPTH = depth_.GetDeviceMaxDepth();
    depth_hist depthHist(MAX_DEPTH);
    unsigned int points = 0;
    const XnDepthPixel* pDepth = depthMD.Data();
    for(XnUInt y=0; y<depthMD.YRes(); ++y) {
        for(XnUInt x=0; x<depthMD.XRes(); ++x, ++pDepth) {
            if(*pDepth != 0) {
                depthHist[*pDepth]++;
                points++;
            }
        }
    }
    
    for(int i=1; i<MAX_DEPTH; ++i) {
        depthHist[i] += depthHist[i-1];
    }
    if(points != 0) {
        for(int i=1; i<MAX_DEPTH; ++i) {
            depthHist[i] = (unsigned int)(256*(1.0f-(depthHist[i]/points)));
        }
    }
    return depthHist;
    
    
    
    
}
