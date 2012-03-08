#include <iostream>
#include "Header.h"
#include <stdio.h>

Header::Header()
{
    //body_ = new unsigned char(8);
}

Header *Header::ParseHeader(std::string &input)
{
    Header* header = new Header();
    
    std::vector<std::string> lines;
    boost::split(lines, input, boost::is_any_of("\r\n"),boost::token_compress_on);
    size_t count = lines.size();
    
    std::vector<std::string> tmp;
    
    boost::algorithm::split(tmp, lines[0], boost::is_space());
    header->setMethod(tmp[0]);
    header->setUri(tmp[1]);
    
    for(int i=1;i<count-1;i++)
    {   
        std::vector<std::string> splited;
        boost::algorithm::split(splited,lines[i], boost::is_any_of(":"));
        if (splited.size()>1) {
            std::string key = boost::algorithm::trim_copy(splited[0]);
            std::string value = splited[1];
            for(int j=2;j<splited.size();j++)
                value += ":"+splited[j];
            boost::algorithm::trim(value);
        
            header->setValue(boost::algorithm::to_lower_copy(key), value);
            std::cout << key << "  " << value << std::endl;
        }
    }
    
    //header->setBody(lines[count-1]);
    //std::cout << "body:" ;
    for(int i=0;i<8;i++){
      //  fprintf(stdout,"0x%02x,",(unsigned char)lines[count-1][i]);
        header->body_[i] = (unsigned char)(lines[count-1][i] & 0xff);
    }
    //header->setBody(body);
    std::cout << std::endl;
    
    return header;
}

//アクセサメソッド
std::string Header::getValue(std::string index)
{
    return headers_[index];
}

void Header::setValue(std::string index, std::string value)
{
    headers_.insert(std::map<std::string,std::string>::value_type(index,value));
}

std::string Header::uri()
{
    return uri_;
}
void Header::setUri(std::string uri)
{
    uri_ = uri;
}

//std::string Header::body()
unsigned char* Header::body()
{
    return body_.data();
}
//void Header::setBody(std::string body)
/*
void Header::setBody(unsigned char *body)
{
    body_ = body;
}
*/
std::string Header::method()
{
    return method_;
}
void Header::setMethod(std::string method)
{
    method_ = method;
}

std::string toString();
