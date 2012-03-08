#include <iostream>
#include "ConnectionFactory.h"

#include "Connection_draft76.h"
#include "Connection_ver10.h"
#include "Header.h"

#include <boost/foreach.hpp>
using boost::asio::ip::tcp;

Connection* ConnectionFactory::createConnection()
{
    boost::array<char, 2048> buffer;
    boost::system::error_code err;
    
    socket_->read_some(boost::asio::buffer(buffer),err);
    
    if(!err)
    {
        std::string input(buffer.data());
        Header *header = Header::ParseHeader(input);
        
        if(!header->getValue("sec-websocket-key").empty())
        {
            std::cout << "ver 10" << std::endl;
            Connection_ver10 *connection = new Connection_ver10(socket_,header);
            return connection;//new Connection_ver10(socket_,header);
        }
        
        if(!header->getValue("sec-websocket-key2").empty())
        {
            std::cout << "draft" << std::endl;
            Connection_draft76 *connection = new Connection_draft76(socket_,header);
            return connection;
        }
        delete header;
    }
    
    return NULL;
}



tcp::socket &ConnectionFactory::socket()
{
    return *socket_;
}

ConnectionFactory::ConnectionFactory(boost::asio::io_service &io_service)
{
    socket_ = socket_ptr(new tcp::socket(io_service));
}
