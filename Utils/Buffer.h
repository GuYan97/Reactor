//
// Created by guyan on 2020/12/11.
//

#ifndef RSVP_BUFFER_H
#define RSVP_BUFFER_H
#include <memory>
namespace rsvp {
/**
 * @brief A simple class of memory block
 */
class Buffer {
public:
    explicit Buffer(int length);
    explicit Buffer(const std::string& str);
    Buffer(const std::shared_ptr<Buffer>& src, int offset, int length);
    ~Buffer();
    char* getBuffer();
    int getLength() const;
    int writeToBuffer(int fd, int length);
    void resetBuffer();
private:
    char* _buffer;
    char* _writePtr;
    int _length;
    int _writeAvailable;
};
}



#endif //RSVP_BUFFER_H
