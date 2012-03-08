#include <iostream>
#include <stdio.h>
#include "Connection_draft76.h"

Connection_draft76::Connection_draft76(socket_ptr socket,Header *request)
:Connection(socket)
{
    
    //ハンドシェイク開始
    current = HAND_SHAKE;
    
    std::string ws_key1 = request->getValue("sec-websocket-key1");
    std::string ws_key2 = request->getValue("sec-websocket-key2");
    
    //受信したkeyのデコード
    key1 = decodeKey(ws_key1);
    key2 = decodeKey(ws_key2);
    
    
    /*レスポンスのBODYを作成
     *  DECODEしたkey1,key2(32bit,ビッグエンディアン),リクエストボディ(64bit)を並べ
     * md5ハッシュ値(16byte)を求める。
     */
    unsigned char* body = request->body();
    boost::array<unsigned char, 16> reply;
    
    for (int i=0; i<4; i++) {
        reply[i  ] = (key1>>((3-i)*8)) & 0xff;
        reply[i+4] = (key2>>((3-i)*8)) & 0xff;
    }
    
    for (int j=0;j<8;j++) {
        reply[8+j] = body[j];
    }

    unsigned char hash[16];
    md5(reply.data(), reply.size(), hash);
    //レスポンスボディ作成　ここまで
    
    //レスポンス内容を作成
    std::string response;
    response = "HTTP/1.1 101 Switching Protocols\r\n";
    response+= "Upgrade: websocket\r\n";
    response+= "Connection: Upgrade\r\n";
    response+= "Sec-Websocket-Origin: "+ request->getValue("origin") + "\r\n";
    response+= "Sec-Websocket-Location: ws://" + request->getValue("host") + request->uri() + "\r\n";
    //response+= "Sec-Websocket-Protocol: " + request.getValue("")
    response+="\r\n";
    for(int j=0;j<16;j++)
        response+= hash[j];
    response+="\0";
    boost::system::error_code err;
    
    //レスポンス送信
    socket_->write_some(boost::asio::buffer(response),err);
    
    if(!err)
    {
        //エラーがなければ、コネクション成立
        current = CONNECTED;
    } else {
        current = DISCONNECTIED;
        socket_->close();
    }
    
    
}

uint32_t Connection_draft76::decodeKey(std::string ws_key)
{
    uint32_t key_value=0;
    uint32_t space_num=0;
    
	std::string::iterator it;
    std::string str_key="";
    
    for(it=ws_key.begin();it!=ws_key.end();it++)
	{
		if(std::isdigit(static_cast<unsigned char>(*it)))
		{
			str_key += *it; 
		}else if((*it)==' '){
			space_num++;
		}
	}
    
    key_value = boost::lexical_cast<uint32_t>(str_key);
    
    return  key_value/space_num;
}




void Connection_draft76::sendData(unsigned char* data, int size)
{
    

}

void Connection_draft76::sendText(std::string input)
{
    const char* input_ary = input.c_str();
    int size = input.size();
    
    unsigned char *output = new unsigned char[size+2];
    
    output[0] = 0x00;
    for(int i=0;i<size;i++)
        output[i+1] = (input_ary[i] & 0xff);
    output[size+1] = 0xff;
    
    socket_->write_some(boost::asio::buffer(output,size+2));
    
    
}
void Connection_draft76::closeConnection()
{
    if(current == CONNECTED){
        char data[2];
        data[0] = 0xff;
        data[1] = 0x00;
        boost::system::error_code e;
    
        socket_->write_some(boost::asio::buffer(data,2),e);
    
        if(!e){
            socket_->close();
            current = DISCONNECTIED;
        }
    }
}

void Connection_draft76::handle_read(const boost::system::error_code &e, std::size_t bytes_transferred)
{
    std::cout << "recive:" << bytes_transferred <<" byte" << std::endl; 

   if(!e){
        std::cout << buffer.data() << std::endl;
        listen();
    } else {
        std::cout << e.message() << std::endl;
        socket_->close();
        current = DISCONNECTIED;
    }
}   
