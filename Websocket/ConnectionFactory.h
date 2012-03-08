#ifndef WebSocketServer_ConnectionFactory_h
#define WebSocketServer_ConnectionFactory_h
#include "Connection.h"

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#define RECIVE_BUFFER_SIZE 2048
using boost::asio::ip::tcp;

class ConnectionFactory: public boost::enable_shared_from_this<ConnectionFactory>
{
    
    
public:
    typedef boost::shared_ptr<ConnectionFactory> pointer;
    static pointer create(boost::asio::io_service &io_service)
    {
        return ConnectionFactory::pointer(new ConnectionFactory(io_service));
    }
    
    Connection *createConnection();
    
    tcp::socket &socket();
    
private:
    ConnectionFactory(boost::asio::io_service &io_service);
    
    boost::asio::io_service *service;
    socket_ptr socket_;
    
};

#endif
