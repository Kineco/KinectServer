#ifndef WebSocketServer_Header_h
#define WebSocketServer_Header_h
#include <string>
#include <map>
#include <vector>

#include <boost/array.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

class Header {
    explicit Header();
    
public:
    static Header* ParseHeader(std::string &input);
    
    //アクセサメソッド
    std::string getValue(std::string index);
    void setValue(std::string index, std::string value);
    
    std::string uri();
    void setUri(std::string uri);
    
    //std::string body();
    unsigned char *body();
    //void setBody(std::string body);
    void setBody(unsigned char *body);
    std::string method();
    void setMethod(std::string method);
    
    std::string toString();
    
private:
    std::map<std::string, std::string> headers_;
    std::string uri_;
    //std::string body_;
    //unsigned char *body_;
    boost::array<unsigned char, 8> body_;
    std::string method_;
};


#endif
