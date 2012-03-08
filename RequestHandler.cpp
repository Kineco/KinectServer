#include "RequestHandler.h"
#include "KinectHttpServer.h"
#include <boost/lexical_cast.hpp>

int RequestHandler::handle_request( void *cls, struct MHD_Connection *connection,
                                   const char *url, const char *method,
                                   const char *version, const char *upload_data,
                                   size_t *upload_data_size, void **con_cls)
{
    RequestHandler *handle = new RequestHandler;
    handle->server_ = static_cast<KinectHttpServer*>(cls);
    handle->connection_ = connection;
    int ret;
    try{
        
        handle->createResponse();
        ret = handle->send();
        
    }catch(std::exception &ex){
        std::cerr << ex.what() << std::endl;
        ret = handle->sendError(ex.what());
    }
    delete handle;
    return ret;
}

RequestHandler::RequestHandler()
{
}
RequestHandler::~RequestHandler()
{
    
}

//リクエストの形式からレスポンスを選択して作成する。
//リクエスト形式が不正な場合は例外を投げる。
void RequestHandler::createResponse()
{
    
    //リクエストのタイプを取得、NULLの場合は例外を投げる
    const char* type = MHD_lookup_connection_value(connection_,MHD_GET_ARGUMENT_KIND,"type");
    std::string reqType;
    if(type!=NULL){
        reqType = type;
    }else {
        throw std::runtime_error("Error Args");
    }
    
    /* getRgbData 
     *  RGBカメラのバイナリデータ(Base64エンコード)取得
     *  API仕様書 2.1項
     */
    if(!reqType.compare("getRgbData")){
        Json::Value root = server_->rgbImage()->getRgbData();
        Json::StyledWriter writer;
        std::string out = writer.write(root);
        res = MHD_create_response_from_buffer(out.size(), (void*)out.c_str(), MHD_RESPMEM_PERSISTENT);
        contentType = MIME_JSON;
        
        
    }
    /* getJpegImage 
     *  RGBカメラのJpegデータ取得
     *  API仕様書 2.2項
     */
    else if(!reqType.compare("getJpegImage")){
        std::vector<unsigned char>buf = server_->rgbImage()->getJpegImage();
        if(buf.size() <= 0)
            throw std::runtime_error("cannot create Jpeg Image");
        res = MHD_create_response_from_buffer(buf.size(), (void*)(&buf[0]), MHD_RESPMEM_MUST_COPY);
        contentType = MIME_JPEG;
    }
    
    else if(!reqType.compare("getDepthImageData")){
        Json::Value root = server_->depthImage()->getDepthImageData();
        Json::StyledWriter writer;
        std::string out = writer.write(root);
        res = MHD_create_response_from_buffer(out.size(), (void*)out.c_str(), MHD_RESPMEM_MUST_COPY);
        contentType = MIME_JSON;
     
        
    }
    else if(!reqType.compare("getDepthImage")){
        std::vector<unsigned char>buf = server_->depthImage()->getDepthImage();
        if(buf.size() <=0)
            throw std::runtime_error("cannot create Jpeg Image");
        res = MHD_create_response_from_buffer(buf.size(), (void*)(&buf[0]), MHD_RESPMEM_MUST_COPY);
        contentType = MIME_JPEG;
    }
    
    else if(!reqType.compare("getPointCloud")){
        Json::Value root = server_->pointCloud()->getPointCloud();
        Json::FastWriter writer;
        std::string out = writer.write(root);
        res = MHD_create_response_from_buffer(out.size(),(void*)out.c_str(),MHD_RESPMEM_MUST_COPY);
        contentType = MIME_JSON;
    }
    
    else if(!reqType.compare("getSkeletonJointPosition")){
        const char *ejoint,*id_;
        ejoint = MHD_lookup_connection_value(connection_, MHD_GET_ARGUMENT_KIND, "eJoint");
        id_ = MHD_lookup_connection_value(connection_, MHD_GET_ARGUMENT_KIND, "user");
        if(ejoint==NULL || id_==NULL)
            throw std::runtime_error("Argment Error");
        Json::Value root = server_->calibratedUsers()->getSkeletonJointPosition(atoi(id_), std::string(ejoint));
        Json::FastWriter writer;
        std::string out = writer.write(root);
        res = MHD_create_response_from_buffer(out.size(), (void*)out.c_str(), MHD_RESPMEM_MUST_COPY);
        contentType=MIME_JSON;
        
    }
    
    else if(!reqType.compare("getCenterPoints")){
        Json::Value root = server_->detectedUsers()->getCenterPoints();
        Json::FastWriter writer;
        std::string out = writer.write(root);
        res = MHD_create_response_from_buffer(out.size(), (void*)out.c_str(), MHD_RESPMEM_MUST_COPY);
        contentType = MIME_JSON;
    }
    else if(!reqType.compare("getUserIds")){
        Json::Value root = server_->detectedUsers()->getUserIds();
        Json::FastWriter writer;
        std::string out = writer.write(root);
        res = MHD_create_response_from_buffer(out.size(), (void*)out.c_str(), MHD_RESPMEM_MUST_COPY);
        contentType = MIME_JSON;
        
    }
    else if(!reqType.compare("getCalibratedUserIds")){
        Json::Value root = server_->calibratedUsers()->getCalibratedUserIds();
        Json::FastWriter writer;
        std::string out = writer.write(root);
        res = MHD_create_response_from_buffer(out.size(), (void*)out.c_str(), MHD_RESPMEM_MUST_COPY);
        contentType = MIME_JSON;
    }
    
    else if(!reqType.compare("getNumberOfDetectedUsers")){
        Json::Value root = server_->detectedUsers()->getNumberOfDetectedUsers();
        Json::FastWriter writer;
        std::string out = writer.write(root);
        res = MHD_create_response_from_buffer(out.size(), (void*)out.c_str(), MHD_RESPMEM_MUST_COPY);
        contentType = MIME_JSON;
    }
    
    else if(!reqType.compare("getNumberOfCalibratedUsers")){
        Json::Value root = server_->calibratedUsers()->getNumberOfCalibratedUsers();
        Json::FastWriter writer;
        std::string out = writer.write(root);
        res = MHD_create_response_from_buffer(out.size(), (void*)out.c_str(), MHD_RESPMEM_MUST_COPY);
        contentType = MIME_JSON;
    }
    
    else if(!reqType.compare("getDepth")){
        const char *x,*y;
        x = MHD_lookup_connection_value(connection_,MHD_GET_ARGUMENT_KIND,"x");
        y = MHD_lookup_connection_value(connection_,MHD_GET_ARGUMENT_KIND,"y");
        if(x==NULL || y==NULL)
            throw std::runtime_error("argment error");
        Json::Value root = server_->depthMap()->getDepth(atoi(x), atoi(y));
        Json::FastWriter writer;
        std::string out = writer.write(root);
        res = MHD_create_response_from_buffer(out.size(), (void*)out.c_str(), MHD_RESPMEM_MUST_COPY);
        contentType = MIME_JSON;
        
    }
    
    else if(!reqType.compare("getSkeletonById")){
        const char *id_;
        id_ = MHD_lookup_connection_value(connection_,MHD_GET_ARGUMENT_KIND,"user");
        if(id_ == NULL)
            throw std::runtime_error("argment error");
        Json::Value root = server_->calibratedUsers()->getSkeletonById(atoi(id_));
        Json::FastWriter writer;
        std::string out = writer.write(root);
        res = MHD_create_response_from_buffer(out.size(), (void*)out.c_str(), MHD_RESPMEM_MUST_COPY);
        contentType = MIME_JSON;
    }
    
    else if(!reqType.compare("getDepthImageData")){
        Json::Value root = server_->depthImage()->getDepthImageData();
        Json::FastWriter writer;
        std::string out = writer.write(root);
        res = MHD_create_response_from_buffer(out.size(), (void*)out.c_str(), MHD_RESPMEM_MUST_COPY);
        contentType = MIME_JSON;
    }
    else if(!reqType.compare("getCenterPointById")){
        const char *id_;
        id_ = MHD_lookup_connection_value(connection_,MHD_GET_ARGUMENT_KIND,"id");
        if(id_ == NULL)
            throw std::runtime_error("argment error");
        Json::Value root = server_->detectedUsers()->getCenterPointById(atoi(id_));
        Json::FastWriter writer;
        std::string out = writer.write(root);
        res = MHD_create_response_from_buffer(out.size(), (void*)out.c_str(), MHD_RESPMEM_MUST_COPY);
        contentType = MIME_JPEG;
    }
    else if(!reqType.compare("getDepthById")){
        const char *id_;
        id_ = MHD_lookup_connection_value(connection_,MHD_GET_ARGUMENT_KIND,"id");
        if(id_ == NULL)
            throw std::runtime_error("argment error");
        Json::Value root = server_->detectedUsers()->getDepthById(atoi(id_));
        Json::FastWriter writer;
        std::string out = writer.write(root);
        res = MHD_create_response_from_buffer(out.size(), (void*)out.c_str(), MHD_RESPMEM_MUST_COPY);
        contentType = MIME_JPEG;
    }
    
    else if(!reqType.compare("getMultiPointCloud")){
        throw std::runtime_error("BAD REQUEST");
        
    }
    else if(!reqType.compare("getRgbDataBinary")){
        std::vector<unsigned char> buf= server_->rgbImage()->getRgbBinary();
        res = MHD_create_response_from_buffer(buf.size(), (void*)(&buf[0]), MHD_RESPMEM_MUST_COPY);
        contentType = "application/actet-stream";
        
    }
    else{
        throw std::runtime_error("BAD REQUEST");
    }
}

int RequestHandler::send()
{
    //クロスドメイン通信を許可する
    MHD_add_response_header(res, "Access-Control-Allow-Origin", "*");
    MHD_add_response_header(res, "Access-Control-Allow-Headers", "*");
    MHD_add_response_header(res, "Access-Control-Allow-Method", "GET, POST, OPTIONS");
    //コンテンツタイプの指定
    MHD_add_response_header(res, MHD_HTTP_HEADER_CONTENT_TYPE, contentType.c_str());
    
    if (contentType==MIME_JPEG) {
        MHD_add_response_header(res, "Content-Disposition", "filename=\"Kineco.jpg\"");
    }
    
    
    int ret = MHD_queue_response(connection_, MHD_HTTP_OK, res);
    MHD_destroy_response(res);
    return ret;
}

int RequestHandler::sendError(std::string message)
{
    res = MHD_create_response_from_buffer(message.size(), (void*)message.c_str(), MHD_RESPMEM_MUST_COPY);
    int ret= MHD_queue_response(connection_, MHD_HTTP_BAD_REQUEST,res);
    MHD_destroy_response(res);
    return ret;
}
