//
// Created by guyan on 2020/12/10.
//

#ifndef RSVP_LISTENHANDLER_H
#define RSVP_LISTENHANDLER_H
#include "../Reactor/EventHandler.h"
#include "Socket.h"
#include <memory>
namespace rsvp {

/**
 * @brief The listen event handler
 * @note When the listen fd receive an EPOLLIN event, create a connection to accept it
 */
class ListenHandler : public EventHandler {
public:
    explicit ListenHandler(int fd);
    ~ListenHandler() override;
    int getFd() override;
    void handleRead() override;
    void handleWrite() override;
    void handleClose() override;
    void handleError() override;
private:
    std::unique_ptr<Socket> _listenSocket;
};
}



#endif //RSVP_LISTENHANDLER_H
