//
// Created by guyan on 2020/12/10.
//

#include "ConnectionHandler.h"

#include <unistd.h>
#include <iostream>
#include <thread>
#include "../Reactor/Reactor.h"
#include "../Utils/ThreadPool.h"
using namespace rsvp;

/**@brief Constructor of connection handler
 * @param fd : the connection socket file descriptor
 */
ConnectionHandler::ConnectionHandler(int fd) :
        _dataToRead(0),
        _readState(READ_HEAD),
        _readAvailable(true) {
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

}

void ConnectionHandler::handleWrite() {
}

/**
 * @brief Handle the close event
 */
void ConnectionHandler::handleClose() {
    Reactor::getInstance().removeHandler(_connSocket->fd());
    perror("socket close");
}

void ConnectionHandler::handleError() {
    Reactor::getInstance().removeHandler(_connSocket->fd());
    perror("socket error");
}

/**
 * @brief Handle the pack data
 * @Note When the head and data pack is read, handle the data by the pack type
 */
void ConnectionHandler::handlePack() {

}
