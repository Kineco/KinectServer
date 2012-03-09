#ifndef WebSocketServer_Connection_ver10_h
#define WebSocketServer_Connection_ver10_h
#include "Connection.h"
#include "Header.h"
#include "PolarSSL/sha1.h"
#include "PolarSSL/base64.h"
#include "Frame.h"

#define GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

using boost::asio::ip::tcp;
class Connection_ver10: public Connection {
    
    std::string ver_;
public:
    explicit Connection_ver10(socket_ptr socket,Header *request);

    void handle_read(const boost::system::error_code& e, std::size_t bytes_transferred);
    std::string ver();
    void sendData(unsigned char* data, int size);
    void sendText(std::string input);
    void closeConnection();
private:
    std::string accept;
    
};

#endif
