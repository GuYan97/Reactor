//
// Created by guyan on 2020/12/10.
//

#ifndef RSVP_CONNECTIONHANDLER_H
#define RSVP_CONNECTIONHANDLER_H
#include "../Reactor/EventHandler.h"
#include "../Utils/Buffer.h"
#include "Socket.h"
#include <memory>
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
private:
    std::unique_ptr<Socket> _connSocket;
    std::shared_ptr<Buffer> _headBuffer;
    std::shared_ptr<Buffer> _dataBuffer;
    int _headLength;

    int _headToRead;
    int _dataToRead;
    ReadState _readState;
    bool _readAvailable;

    std::mutex _mutex;

    void handlePack();
};

}


#endif //RSVP_CONNECTIONHANDLER_H
