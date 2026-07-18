#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <cstddef>
#include <string>
class Buffer{
private:
    std::string m_buffer;

public:
    Buffer();
    ~Buffer();
    void append(const char* _str,size_t _size);
    void clear();
    const char* c_str();
    size_t getsize();
};

#endif