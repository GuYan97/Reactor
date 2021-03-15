//
// Created by guyan on 2020/12/11.
//

#include "Buffer.h"
#include <unistd.h>
#include <cstdio>
using namespace rsvp;

/**
 * @brief Constructor of Buffer
 * @param length : the length of Buffer
 */
Buffer::Buffer(int length) :
        _length(length),
        _buffer(new char[length]),
        _writeAvailable(length) {
    _writePtr = _buffer;
}

Buffer::Buffer(const std::string& str) :
        _length(str.length()),
        _writeAvailable(str.length()){
    _buffer = new char[_length];
    char* ptr = _buffer;
    for(auto i : str)
        *(ptr++) = i;
    _writePtr = _buffer;
}


/**
 * @brief Constructor of Buffer
 * @note Create a buffer, the data is a part of src buffer
 * @param src : data src pointer
 * @param offset : the offset of data
 * @param length  : the length of data
 */
Buffer::Buffer(const std::shared_ptr<Buffer>& src, int offset, int length) :
        _length(length),
        _buffer(new char[length]),
        _writeAvailable(length) {
    auto srcPtr = src->getBuffer() + offset;

    for(auto i  = 0; i < length; i++) {
        _buffer[i] = *(srcPtr++);
    }
    _writePtr = _buffer;
}

/**
 * @brief Destructor of Buffer
 */
Buffer::~Buffer() {
    delete[](_buffer);
}

/**
 * @brief Acquire the buffer pointer
 * @return the buffer pointer
 */
char* Buffer::getBuffer() {
    return _buffer;
}

/**
 * @brief Acquire the buffer length
 * @return the buffer lenth
 */
int Buffer::getLength() const {
    return _length;
}

/**
 * @brief Read data from file to buffer
 * @param fd : the file descriptor
 * @param length : the length of data to read
 * @return the actual read data length
 */
int Buffer::writeToBuffer(int fd, int length) {
    if(length > _writeAvailable) {
        perror("write length error");
        return 0;
    }
    int len = read(fd, _writePtr, length);
    if(len <= 0)
        return 0;
    _writePtr += len;
    _writeAvailable -= len;
    return len;
}

/**
 * @brief Reset the write pointer to the begin of buffer
 */
void Buffer::resetBuffer() {
    _writePtr = _buffer;
    _writeAvailable = _length;
}



