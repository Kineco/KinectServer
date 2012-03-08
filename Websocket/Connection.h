#ifndef WebSocketServer_Connection_h
#define WebSocketServer_Connection_h
#include <map>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "Header.h"

using boost::asio::ip::tcp;

class Connection;

typedef boost::shared_ptr<Connection> connection_ptr ;
typedef boost::shared_ptr<tcp::socket> socket_ptr;

class Connection:public boost::enable_shared_from_this<Connection>
{
    std::map<std::string,std::string> requestHeader;
    
public:
    explicit Connection(socket_ptr socket);
    void listen();

    
    virtual void handle_read(const boost::system::error_code& e, std::size_t bytes_transferred)=0;
    virtual void sendData(unsigned char *data, int size)=0;
    virtual void sendText(std::string input)=0;
    virtual void closeConnection()=0;
    
    tcp::socket &socket();
    typedef enum{
        HAND_SHAKE,
        CONNECTED,
        SENDING,
        DISCONNECTIED
    }State;
    
    State Status();
    
    
    
protected:
    socket_ptr socket_;
    boost::array<char, 2048> buffer;
    State current;
};


#endif
