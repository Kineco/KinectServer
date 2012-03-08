#include <iostream>
#include "Connection_ver10.h"

Connection_ver10::Connection_ver10(socket_ptr socket,Header *request)
:Connection(socket)
{
    current = HAND_SHAKE;
    
    std::string key = request->getValue("sec-websocket-key") + GUID;
    
    unsigned char buf[30],hash[20];
    
    sha1((unsigned char*)key.c_str(), key.size(), hash);
    std::cout <<"key:"<< key <<std::endl;
    size_t output_size;
    base64_encode(buf, &output_size, hash, 20);
    
    accept = (char*)buf;
    
    
    std::string response;
    response  = "HTTP/1.1 101 Switching Protocols\r\n";
    response += "Upgrade: websocket\r\n";
    response += "Connection: Upgrade\r\n";
    response += "Sec-WebSocket-Accept: " + accept + "\r\n\r\n";
    
    std::cout << response << std::endl;
    boost::system::error_code e;
    socket_->write_some(boost::asio::buffer(response),e);
    
    if(!e){
        current = CONNECTED;
    }else {
        current = DISCONNECTIED;
        socket_->close();
    }
    
}

std::string Connection_ver10::ver()
{
    return ver_;
}

void Connection_ver10::sendData(unsigned char* data, int size)
{
    std::cout << ver_ << std::endl;
}

void Connection_ver10::sendText(std::string input)
{
    Frame *frame = Frame::CreateTextFrame(input);
    
    unsigned char *out = frame->ToBinary();
    int frame_size = frame->FrameSize();
    boost::system::error_code e;

    current = SENDING;
    socket_->write_some(boost::asio::buffer(out,frame_size),e);
    if(!e){
        current = CONNECTED;
    }else {
        current = DISCONNECTIED;
        std::cerr << "connection: " << e.message() << std::endl;
        socket_->close();
    }
}

void Connection_ver10::closeConnection()
{
    
}

void Connection_ver10::handle_read(const boost::system::error_code &e, std::size_t bytes_transferred)
{
    std::cout << "recive:" << bytes_transferred <<" byte" << std::endl; 
    if(!e){
        
        Frame *frame = Frame::ParseFrame(buffer.data());
        switch (frame->Opecode()) {
            case 0x1: //Text Frame
                std::cout << "Recv:" << frame->PayloadData() << std::endl;
                break;
            case 0x8:
                std::cout << "Connection Close" << std::endl;
                socket_->close();
                current = DISCONNECTIED;
            default:
                break;
        }
        
        delete frame;
    }
    else {
        std::cout << e.message() << std::endl;
        socket_->close();
    }
}   
