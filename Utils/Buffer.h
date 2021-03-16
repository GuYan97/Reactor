#ifndef RSVP_BUFFER_H
#define RSVP_BUFFER_H
#include <memory>
namespace rsvp {
/**
 * @brief A simple class of memory block
 */
class Buffer {
public:
    static const size_t prependSize = 12;

    explicit Buffer(size_t length);
    ~Buffer();
    char* getBuffer();
    size_t getLength() const;
    size_t getReadable() const;
    size_t getWritable() const;
    size_t getPrependable() const;
    char* getBuffer() const;
    bool empty();
    void retrieve(size_t len);
    void retrieveAll();
    std::string retrieveAsString(size_t len);
    void append(const char* data, size_t len);
    void append(const std::string& str);
    ssize_t appendFromFd(int fd, size_t len);
    void prepend(const char* data, size_t len);
private:
    size_t _length;
    char* _buffer;
    char* _writePtr;
    char* _readPtr;
};
}



#endif //RSVP_BUFFER_H
