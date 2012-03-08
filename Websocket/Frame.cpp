#include "Frame.h"

Frame::Frame()
{
    fin = false;
    mask = false;
    length = 0;
    for(int i=0;i<4;i++)
        maskingKey[i] = 0x00;
}

Frame::~Frame()
{
    delete[] data;
}

Frame* Frame::ParseFrame(char *input)
{
    Frame *frame = new Frame();
    
    frame->opecode = (int)(input[0] & 0x0f);
    frame->fin = input[0] &  0x80 ? true : false;
    frame->mask = input[1] & 0x80 ? true : false;
    frame->length = (int)(input[1] & 0x7f);
    
    char *data_pos = &input[2];
    
    if(frame->length > 125)
    {
        char *ext_len_pos = data_pos;
        
        if (frame->length == 126) {
            data_pos += 2;
        }else if(frame->length == 127){
            data_pos += 8;
        }
        frame->length = 0;
        
        for(char *pos = ext_len_pos; pos <data_pos ; pos++)
        {
            frame->length <<= 8;
            frame->length |= *pos;
        }
        
    }

    
    if(frame->mask){
        for(int j=0;j<4;j++)
            frame->maskingKey[j] = *(data_pos+j);
        data_pos+=4;
    }
    
    frame->data = new char[frame->length];
    
    for(int i=0;i<frame->length;i++)
    {
        frame->data[i] = data_pos[i];
    }
    
    return frame;
}

Frame* Frame::CreateTextFrame(std::string input)
{
    unsigned long size_ = input.size();
    const char * input_ary = input.c_str();
    Frame *frame = new Frame();
    
    frame->opecode = 0x01;
    frame->fin = true;
    frame->mask = false;
    frame->length = size_;
    
    frame->data = new char[size_];
    for (int i=0; i<size_; i++) {
        frame->data[i] = input_ary[i];
    }
    return frame;
}

Frame* Frame::CreateDataFrame(char *data_, int size)
{
    Frame *frame = new Frame();
    
    frame->opecode = 0x02;
    frame->fin = true;
    frame->mask = false;
    frame->length = size;
    
    frame->data = new char[size];
    for(int i=0;i<size;i++)
    {
        frame->data[i] = data_[i];
    }
    
    return frame;
}

unsigned char* Frame::ToBinary()
{
    int frame_size = FrameSize();
    unsigned char* output = new unsigned char[frame_size];
    uint64_t index = 0;
    
    //finフラグ
    output[index] = fin ? 0x80 : 0x00;
    //opcode
    output[index] += (unsigned char)(opecode & 0x0f);
    index++;
    //mask
    output[index] = mask ? 0x80 : 0x00;
    
    //payload length
    if(length < 126){
        //lenghtが7bit以下の場合
        output[index] += (unsigned char)(length & 0x7f);
        index++;
    } else if(length < 65536){
        //lengthが7bit以上、23bit以下の場合
        //payload lengthは126,その後のExtended payload length 2byteに
        //lengthを入れる
        output[index] = 126;
        index++;
        //lengthはNetwork Endian(Big Endian)で入れる
        output[index] = (unsigned char)(length>>8 & 0xff);
        index++;
        output[index] = (unsigned char)(length & 0xff);
        index++;
    } else {
        //lengthが23bit以上の場合
        //payload lenghtには127,その後のExtended payload length 4byteに
        //lenghtを入れる
        output[index] = 127;
        index++;
        for (int i=0; i<8; i++) {
            output[index] = (unsigned char)(length >> (8 -i*8) & 0xff);
            index++;
        }
        
    }
    
    //masking-key
    if(mask){
        for (int i=0; i<4; i++,index++) {
            output[index] = maskingKey[i];
        }
    }
    
    //payload data
    for (int i=0; i<length; i++,index++) {
        output[index] = data[i];
    }
    
    if(index != frame_size)
    {
        std::cout << "error" << std::endl;
        std::cout << "index: " << index << "  framesize:" << frame_size << std::endl;
    }
    
    return output;
    
    
}

int Frame::FrameSize()
{
    int frame_size = 0;
    
    frame_size += 2;
    if(length > 126)
        frame_size += 2;
    if(length > 65536)
        frame_size += 6;
    
    if(mask)
        frame_size += 2;
    
    frame_size += length;
    
    return frame_size;
}

int Frame::Opecode()
{
    return opecode;
}

unsigned char* Frame::PayloadData()
{
    unsigned char *output = new unsigned char[length+1];
    
    //データのマスキングを解除、マスクが指定されていない場合は
    //maskingKeyが0なので、データはそのまま。
    for(int i=0;i<length;i++)
        output[i] = data[i] ^ maskingKey[i%4];
    if(opecode==0x1)
        output[length] = '\0';
    
    return output;
}

