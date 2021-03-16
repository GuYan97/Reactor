#include "Buffer.h"
#include <unistd.h>
#include <iostream>
using namespace rsvp;

/**
 * @brief Constructor of Buffer
 * @param length : the length of Buffer
 */
Buffer::Buffer(size_t length) :
        _length(length),
        _buffer(new char[length + prependSize]),
        _writePtr(_buffer + prependSize),
        _readPtr(_buffer + prependSize) {
}

/**
 * @brief Destructor of Buffer
 */
Buffer::~Buffer() {
    delete[](_buffer);
}

/**
 * @brief Acquire the buffer posize_ter
 * @return the buffer posize_ter
 */
char* Buffer::getBuffer() {
    return _readPtr;
}

bool Buffer::empty() {
    return _writePtr == _readPtr;
}

/**
 * @brief Acquire the buffer length
 * @return the buffer lenth
 */
size_t Buffer::getLength() const {
    return _length;
}

size_t Buffer::getReadable() const {
    return _writePtr - _readPtr;
}

size_t Buffer::getWritable() const {
    return getLength() - getReadable();
}

size_t Buffer::getPrependable() const {
    return _readPtr - _buffer;
}

char *Buffer::getBuffer() const {
    return _buffer;
}

void Buffer::retrieve(size_t len) {
    if(len < getReadable())
        _readPtr += len;
    else
        retrieveAll();
}

void Buffer::retrieveAll() {
    _readPtr = _buffer + prependSize;
    _writePtr = _readPtr;
}

std::string Buffer::retrieveAsString(size_t len) {
    std::string res(_readPtr, len > getReadable()? getReadable() : len);
    retrieve(len);
    return res;
}

void Buffer::append(const char *data, size_t len) {
    if(len > getWritable()) {
        std::cout << "write not available" << std::endl;
        return;
    }

    std::copy(data, data + len, _writePtr);
    _writePtr += len;
}

void Buffer::append(const std::string &str) {
    append(str.data(), str.size());
}

ssize_t Buffer::appendFromFd(const int fd, const size_t len) {
    ssize_t readLength = len > getWritable()? getWritable() : len;
    ssize_t res = read(fd, _writePtr, readLength);
    if(res > 0)
        _writePtr += res;
    return res;
}
void Buffer::prepend(const char *data, size_t len) {
    if(len > getPrependable()) {
        std::cout << "prepend not available" << std::endl;
        return;
    }
    _readPtr -= len;
    std::copy(data, data + len, _readPtr);
}

