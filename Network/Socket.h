//
// Created by guyan on 2020/12/2.
//

#ifndef RSVP_SOCKET_H
#define RSVP_SOCKET_H
#include <sys/socket.h>
#include <netinet/in.h>

namespace rsvp {
/**
 * @brief A package of socket
 */
class Socket {
public:
    explicit Socket(int sockfd) : _sockfd(sockfd) {}
    ~Socket();
    int fd() const;
    bool getSocketInfo(struct tcp_info* info) const;
    void bindAddress(const struct sockaddr_in* addr) const;
    void listen() const;
    int accept(struct ::sockaddr_in* addr) const;

    void setTcpNoDelay(bool on) const;

    void setKeepAlive(bool on) const;

private:
    int _sockfd;
};


}


#endif //RSVP_SOCKET_H
