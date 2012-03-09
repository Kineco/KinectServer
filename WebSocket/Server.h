#ifndef WebSocketServer_Server_h
#define WebSocketServer_Server_h
#include "Connection.h"
#include "ConnectionFactory.h"


#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>


//using boost::asio::ip::tcp;
class Server : private boost::noncopyable
{
public:
    explicit Server(int port);
    void broadcastText(std::string text);
    void broadcastData(unsigned char *data , int size);
    void run();
private:
    void start_accept();
    void handle_accept(ConnectionFactory::pointer connection, const boost::system::error_code &error);
    
    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    
    ConnectionFactory::pointer newConnection;
    std::vector<connection_ptr> connections;
    boost::mutex mtx;
};
#endif
