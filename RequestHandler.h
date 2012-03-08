#ifndef KinecoHTTP_RequestHandler_h
#define KinecoHTTP_RequestHandler_h
#include <microhttpd.h>
#include <sys/types.h>
#include <iostream>
#include <stdexcept>
#include <vector>


#define MIME_JPEG "image/jpg"
#define MIME_JSON "application/json"
#define MIME_TEXT "text/plain"

class KinectHttpServer;

class RequestHandler {

public:
    static int handle_request( void *cls, struct MHD_Connection *connection,
                              const char *url, const char *method,
                              const char *version, const char *upload_data,
                              size_t *upload_data_size, void **con_cls);
    
    explicit RequestHandler();
    ~RequestHandler();
    
private:
    /* createResponse
     *  リクエストによって処理を振り分け、レスポンスを作成する。
     */
    void createResponse();
    /* send
     *  作成されたレスポンスを送信する。
     */
    int send();
    /* sendError
     *  クライアントにエラーを送信する。
     */
    int sendError(std::string message);
    
    
    KinectHttpServer *server_;
    struct MHD_Connection *connection_;
    const char *url_;
    
    struct MHD_Response *res;
    std::string contentType;
    
    
    /* レスポンスの内容を保持するバッファー */
    std::vector<unsigned char> buffer;
    
};

#endif
