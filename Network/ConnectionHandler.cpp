#include "ConnectionHandler.h"
#include <unistd.h>
#include <iostream>
#include <thread>
#include "../Reactor/Reactor.h"
#include "../Utils/ThreadPool.h"
#include "Definations.h"
#include <opencv2/opencv.hpp>
using namespace rsvp;

/**@brief Constructor of connection handler
 * @param fd : the connection socket file descriptor
 */
ConnectionHandler::ConnectionHandler(int fd) {
    std::unique_ptr<Socket> p(new Socket(fd));
    _connSocket = move(p);
}

/**
 * @brief Destructor of connection handler
 * @note This will close the connect socket
 */
ConnectionHandler::~ConnectionHandler() {
    close(_connSocket->fd());
}

/**
 * @brief Acquire the file descriptor
 * @return file descriptor
 */
int ConnectionHandler::getFd() {
    return _connSocket->fd();
}

/**
 * @brief Handle the read event
 */
void ConnectionHandler::handleRead() {
    static ReadState readState = READ_HEAD;
    static size_t headToRead, dataToRead;

    static std::shared_ptr<Buffer> headBuffer = std::make_shared<Buffer>(sizeof(struct head));
    static std::shared_ptr<Buffer> dataBuffer;
    bool readAvailable = true;
    headToRead = sizeof(struct head);
    while(readAvailable) {
        ssize_t n = 0;
        switch (readState) {
            case READ_HEAD:
                n = headBuffer->appendFromFd(_connSocket->fd(), sizeof(struct head));
                if(n <= 0) {
                    readAvailable = false;
                    return;
                }
                if(n < headToRead) {
                    headToRead -= n;
                    readAvailable = false;
                } else {
                    const head* headPointer = (head*)headBuffer->getBuffer();
                    headToRead = 0;
                    dataToRead = headPointer->dataLength;
                    if(dataToRead == 0) {
                        std::cout << "head received, type:" << headPointer->packType << " length:"
                                    << headPointer->dataLength << std::endl;
                        headBuffer->retrieveAll();
                    } else {
                        readState = READ_DATA;
                        dataBuffer = std::make_shared<Buffer>(headPointer->dataLength);
                    }
                }
                break;
            case READ_DATA:
                n = dataBuffer->appendFromFd(_connSocket->fd(), dataToRead);
                if(n < dataToRead) {
                    dataToRead -= n;
                    readAvailable = false;
                } else {
                    dataToRead = 0;
                    headToRead = sizeof(struct head);
                    readState = READ_HEAD;
                    std::cout << _connSocket->fd() << " pack received: " << dataToRead << std::endl;

                    static int num = 0;
                    cv::Mat image;
                    if((num++)%2)
                        image = cv::imread("/home/guyan/pictures/1.png", cv::IMREAD_UNCHANGED);
                    else
                        image = cv::imread("/home/guyan/pictures/2.png", cv::IMREAD_UNCHANGED);
                    image = image(cv::Rect(0, 329, 2432, 896));
                    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
                    struct head imagePack{};
                    imagePack.packType = PACK_PICTURE;
                    imagePack.dataLength = image.cols * image.rows * image.depth() * image.channels();
                    std::shared_ptr<Buffer> img = std::make_shared<Buffer>(imagePack.dataLength);
                    img->append((char*)(image.data), imagePack.dataLength);
                    img->prepend((const char*) &imagePack, sizeof(struct head));
                    this->sendMessage(img);
                    headBuffer->retrieveAll();
                }
                break;
            default:
                break;
        }
    }
}

void ConnectionHandler::handleWrite() {
    std::unique_lock<std::mutex> lock(_writeMutex);
    while(!_bufferQueue.empty()) {
        std::shared_ptr<Buffer> buffer = _bufferQueue.front();
        size_t n = write(_connSocket->fd(), buffer->getBuffer(), buffer->getReadable());
        if(n < 0) {
            std::cout << "write error" << std::endl;
            break;
        }
        std::cout << "written bytes: " << n << std::endl;
        buffer->retrieve(n);
        if(buffer->empty())
            _bufferQueue.pop();
        else
            break;
    }
}

/**
 * @brief Handle the close event
 */
void ConnectionHandler::handleClose() {
    Reactor::getInstance().removeHandler(_connSocket->fd());
    std::cout << "socket close" << std::endl;
}

void ConnectionHandler::handleError() {
    Reactor::getInstance().removeHandler(_connSocket->fd());
    std::cout << "socket error" << std::endl;
}

void ConnectionHandler::sendMessage(const std::shared_ptr<Buffer>& buffer) {
    std::unique_lock<std::mutex> lock(_writeMutex);
    if(_bufferQueue.empty()) {
        size_t n = write(_connSocket->fd(), buffer->getBuffer(), buffer->getReadable());
        if(n < 0) {
            std::cout << "write error" << std::endl;
            return;
        }
        buffer->retrieve(n);
        if(!buffer->empty())
            _bufferQueue.push(buffer);
    } else
        _bufferQueue.push(buffer);
}

