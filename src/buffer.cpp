#include "buffer.h"
#include <cstddef>
Buffer::Buffer(){

}

Buffer::~Buffer(){

}

void Buffer::append(const char* _str,size_t _size){
    for(int i=0;i<_size;i++){
        m_buffer.push_back(_str[i]);
    }
}

void Buffer::clear(){
    m_buffer.clear();
}

const char* Buffer::c_str(){
    return m_buffer.c_str();
}

size_t Buffer::getsize(){
    return m_buffer.size();
}