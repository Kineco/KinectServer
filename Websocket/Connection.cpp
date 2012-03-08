#include <iostream>
#include "Connection.h"

Connection::Connection(socket_ptr socket)
{
    socket_ = socket;
}

void Connection::listen()
{
    
    socket_->async_read_some(boost::asio::buffer(buffer),
                            boost::bind(&Connection::handle_read,                        
                                        shared_from_this(),
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));

}

Connection::State Connection::Status()
{
    return current;
}

void Connection::handle_read(const boost::system::error_code &e, std::size_t bytes_transferred)
{
    std::cout << "Connection" << std::endl;
    std::cout << e.message() << std::endl;
    //listen();
}

tcp::socket &Connection::socket()
{
    return *socket_;
}
