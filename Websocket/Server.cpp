#include <iostream>
#include <boost/array.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <signal.h>
#include "Server.h"

using boost::asio::ip::tcp;
typedef boost::mutex::scoped_lock lock;
Server::Server(int port)
: io_service_(),acceptor_(io_service_,tcp::endpoint(tcp::v4(),port))
{
    start_accept();
}
void Server::run()
{
    boost::thread thr(boost::bind(&boost::asio::io_service::run,&io_service_)); 
}

void Server::start_accept()
{
    newConnection = ConnectionFactory::create(io_service_);
    acceptor_.async_accept(newConnection->socket(), boost::bind(&Server::handle_accept, 
                                                                this,
                                                                newConnection,
                                                                boost::asio::placeholders::error));
}

void Server::handle_accept(ConnectionFactory::pointer connection, const boost::system::error_code &error)
{

    connection_ptr con = connection_ptr(connection->createConnection());
    con->listen();
    connections.push_back(con);
 
    start_accept();
}

void Server::broadcastData(unsigned char *data, int size)
{   
    BOOST_FOREACH(connection_ptr con,connections)
    {
        lock(mtx);
        con->sendData(data, size);
    }
}

void Server::broadcastText(std::string text)
{
    lock(mtx);
    BOOST_FOREACH(connection_ptr con, connections)
    {
        
        if(con->Status()== Connection::CONNECTED)
            con->sendText(text);
    }
}
