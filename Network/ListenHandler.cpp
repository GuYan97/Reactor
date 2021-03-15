//
// Created by guyan on 2020/12/10.
//

#include "ListenHandler.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory>
#include "../Reactor/Reactor.h"
#include "ConnectionHandler.h"
#include <sys/epoll.h>
#include <iostream>
#include <unistd.h>
#include <memory.h>
using namespace rsvp;

/**
 * @brief Constructor of listen handler
 * @param fd : the listen socket file descriptor
 */
ListenHandler::ListenHandler(int fd) {
    std::unique_ptr<Socket> p(new Socket(fd));
    _listenSocket = move(p);

    struct ::sockaddr_in servaddr{};
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8000);

    _listenSocket->bindAddress(&servaddr);
    _listenSocket->listen();
}

/**
 * @brief Destructor of listen handler
 * this will close the listen socket
 */
ListenHandler::~ListenHandler() {
    close(_listenSocket->fd());
}

/**
 * @brief Acquire the listen socket file descriptor
 * @return file descriptor
 */
int ListenHandler::getFd() {
    return _listenSocket->fd();
}

/**
 * @brief Handle the read event
 */
void ListenHandler::handleRead() {
    struct sockaddr_in *addr = nullptr;
    int fd = _listenSocket->accept(addr);
    std::cout<<"accept fd:"<<fd<<std::endl;
    std::shared_ptr<ConnectionHandler> handler = std::make_shared<ConnectionHandler>(fd);
    Reactor::getInstance().registerHandler(handler, EPOLLIN);
}

void ListenHandler::handleWrite() {
}

void ListenHandler::handleClose() {
}

void ListenHandler::handleError() {
}

