#ifndef WebSocketServer_Connection_draft76_h
#define WebSocketServer_Connection_draft76_h
#include "Connection.h"
#include "Header.h"
#include "PolarSSL/md5.h"
#include <stdint.h>

#include <boost/lexical_cast.hpp>

#define MD5_OUTPUT_SIZE 16

using boost::asio::ip::tcp;
class Connection_draft76: public Connection {
    
    std::string ver_;
public:
    explicit Connection_draft76(socket_ptr socket,Header *request);
    
    //void listen();
    void handle_read(const boost::system::error_code& e, std::size_t bytes_transferred);
    std::string ver();
    void sendData(unsigned char* data, int size);
    void sendText(std::string input);
    void closeConnection();
    
private:
    uint32_t decodeKey(std::string ws_key);
    
    uint32_t key1;
    uint32_t key2;
    
    
};



#endif
