#include "RgbImage.h"
#define Ver_ 1.0


/*コンストラクタ*/
RgbImage::RgbImage(xn::ImageGenerator &image)
:image_(image){
    
    image_.GetMetaData(imageMD);
    wid = imageMD.XRes();
    hei = imageMD.YRes();
    
}

Json::Value RgbImage::getRgbData(){
    printf("now we are sending rgb data ----------------------->:\n");
    IplImage* img;
    img = cvCreateImage(cvSize(wid, hei), IPL_DEPTH_8U, 3);
    //色変換 BGR -> RGBの為に、IplImageに保存
    mutex.lock();
    memcpy(img->imageData, imageMD.RGB24Data(), img->imageSize);
    mutex.unlock();
    
    cvCvtColor(img, img, CV_BGR2RGB);
    unsigned char *tmp = new unsigned char[(int)(img->imageSize * 2)];
    const unsigned char *src = (unsigned char*)(img->imageData);
    size_t output_size = img->imageSize * 2;
    base64_encode(tmp, &output_size, src, img->imageSize);
    tmp[output_size] = '\0';
    
    
    /* 出力用のJSON形式に */
    Json::Value root,kineco,config;
    config["width"] = wid;
    config["height"] = hei;
    
    kineco["version"] = Ver_;
    kineco["config"] = config;
    kineco["pixels"] = std::string(reinterpret_cast<const char*>(tmp),output_size);
    
    root["Kineco"] = kineco;
    return root;
    
}

std::vector<unsigned char> RgbImage::getJpegImage(){
    IplImage* img;
    img = cvCreateImage(cvSize(wid, hei), IPL_DEPTH_8U, 3);
    mutex.lock();
    memcpy(img->imageData, imageMD.RGB24Data(), img->imageSize);
    mutex.unlock();
    cvCvtColor(img, img, CV_BGR2RGB);
    std::vector<unsigned char> buff;
    
    std::vector<int> param = std::vector<int>(2);
    cv::Mat mat(img);
    param[0] = CV_IMWRITE_JPEG_QUALITY;
    param[1] = 95;
    cv::imencode(".jpg", mat, buff);
    
    return buff;
    
    
}

std::vector<unsigned char> RgbImage::getRgbBinary(){
    printf("now we are sending rgb data ----------------------->:\n");
    IplImage* img;
    img = cvCreateImage(cvSize(wid, hei), IPL_DEPTH_8U, 3);
    
    mutex.lock();
    memcpy(img->imageData, imageMD.RGB24Data(), img->imageSize);
    mutex.unlock();
    
    cvCvtColor(img, img, CV_BGR2RGB);
    
    std::vector<unsigned char> out(img->imageData, img->imageData + img->imageSize);
    return out;
    
    
}


void RgbImage::update(){
    boost::mutex::scoped_lock lk(mutex);
    image_.GetMetaData(imageMD); 
}




