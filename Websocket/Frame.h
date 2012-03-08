#ifndef WebSocketServer_Frame_ver10_h
#define WebSocketServer_Frame_ver10_h

#include <boost/array.hpp>
#include <stdint.h>
class Frame {
    int opecode;
    bool fin;
    bool mask;
    unsigned long length;
    char maskingKey[4];
    char *data;
    
public:
    explicit Frame();
    ~Frame();
    static Frame* ParseFrame(char *input);
    static Frame* CreateTextFrame(std::string input);
    static Frame* CreateDataFrame(char* data_,int size);
    
    unsigned char* ToBinary();
    int FrameSize();
    int Opecode();
    unsigned char* PayloadData();
    
private:

};


#endif
