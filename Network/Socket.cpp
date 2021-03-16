//
// Created by guyan on 2020/12/2.
//

#include "Socket.h"
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

using namespace rsvp;

/**
 * @brief The destructor of Socket
 */
Socket::~Socket() {
    close(_sockfd);
}

/**
 * @brief Acquire the file descriptor of the socket
 * @return the file descriptor
 */
int Socket::fd() const{
    return _sockfd;
}

/**
 * @brief Acquire the information of the socket
 * @param info : the information struct
 * @return
 *      @retval 0 : fail
 *      @retval 1 : success
 */
bool Socket::getSocketInfo(struct tcp_info* info) const {
    socklen_t len = sizeof(::tcp_info);
    memset(info, 0, len);
    return getsockopt(_sockfd, SOL_TCP, TCP_INFO, info, &len) == 0;

}

/**
 * @brief Bind the address to the socket
 * @param addr : the address struct
 */
void Socket::bindAddress(const struct sockaddr_in* addr) const{
    int ret = bind(_sockfd, (struct sockaddr*)addr, sizeof(::sockaddr_in));
    if(ret < 0)
        std::cout << "bind fail:" << ret << std::endl;
}

/**
 * @brief Listen to the address and port
 */
void Socket::listen() const{
    int ret = ::listen(_sockfd, SOMAXCONN);
    if(ret < 0)
        std::cout << "listen fail: " << ret << std::endl;
}

/**
 * @brief Accept a connection
 * @param addr : the struct to save peer address
 * @return the file descriptor of connection
 */
int Socket::accept(struct sockaddr_in* addr) const{
    socklen_t addrlen = sizeof(struct sockaddr_in);
    int connfd = ::accept4(_sockfd, (struct sockaddr *)(addr), &addrlen, SOCK_NONBLOCK);
    if(connfd < 0)
        std::cout << "accept fail:" << connfd << std::endl;
    return connfd;
}

/**
 * @brief Set socket to no delay mode
 * @param on : 1 for on, 0 for off
 */
void Socket::setTcpNoDelay(bool on) const{
    int optval = on ? 1 : 0;
    setsockopt(_sockfd, IPPROTO_TCP, TCP_NODELAY,
                 &optval, static_cast<socklen_t>(sizeof optval));
}

/**
 * @brief Set socket to long connection
 * @param on : 1 for on, 0 for off
 */
void Socket::setKeepAlive(bool on) const{
    int optval = on ? 1 : 0;
    setsockopt(_sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, static_cast<socklen_t>(sizeof optval));
}
