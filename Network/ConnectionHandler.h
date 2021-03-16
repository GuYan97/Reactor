//
// Created by guyan on 2020/12/10.
//

#ifndef RSVP_CONNECTIONHANDLER_H
#define RSVP_CONNECTIONHANDLER_H
#include "../Reactor/EventHandler.h"
#include "../Utils/Buffer.h"
#include "Socket.h"
#include <memory>
#include <queue>
#include <mutex>
namespace rsvp {

/**
 * @brief The connect event handler
 * @note When the connection socket fd receive an EPOLLIN event, handle the read data
 */
class ConnectionHandler : public EventHandler,
                            std::enable_shared_from_this<ConnectionHandler> {
    enum ReadState {
        READ_HEAD,
        READ_DATA,
    };
public:
    explicit ConnectionHandler(int fd);
    ~ConnectionHandler() override;
    int getFd() override;
    void handleRead() override;
    void handleWrite() override;
    void handleClose() override;
    void handleError() override;

    void sendMessage(const std::shared_ptr<Buffer>& buffer);
private:
    std::unique_ptr<Socket> _connSocket;

    std::queue<std::shared_ptr<Buffer>> _bufferQueue;
    std::mutex _writeMutex;
};

}


#endif //RSVP_CONNECTIONHANDLER_H
