//
// Created by guyan on 2020/12/10.
//

#include "Reactor.h"
#include "EpollDemultiplexer.h"
using namespace rsvp;

/**
 * @brief Constructor of Reactor
 */
Reactor::Reactor() :
        _demultiplexer(new EpollDemultiplexer()) {
}

/**
 * @brief Destructor of Reactor
 */
Reactor::~Reactor() = default;

/**
 * @brief Register a handler to the reactor
 * @param handler : the event handler to register
 * @param event : the type of event to register
 * @return
 *      @retval 0 : add success
 *      @retval other : add error
 */
int Reactor::registerHandler(const std::shared_ptr<EventHandler>& handler, int event) {
    int fd = handler->getFd();
    if(_handlers.end() == _handlers.find(fd))
        _handlers.insert(std::make_pair(fd, handler));

    return _demultiplexer->registerEvent(fd, event);
}

/**
 * @brief Remove a handler to the reactor.
 * @note Actually when the connection has been closed, and the file descriptor has been erased from
 * EventHandler, the file descriptor will be delete from epoll event automatically. So the
 * removeEvent function just decreased the file number.
 * @param handler : the event handler to remove
 * @return
 *      @retval 0 : remove success
 *      @retval other : remove error
 */
int Reactor::removeHandler(const std::shared_ptr<EventHandler>& handler) {
    int fd = handler->getFd();
    int ret = 0;
    if(_handlers.end() != _handlers.find(fd)) {
        ret = _demultiplexer->removeEvent(fd);
        _handlers.erase(fd);
    }
    return ret;
}

/**
 * @brief Remove a handler to the reactor.
 * @note Actually when the connection has been closed, and the file descriptor has been erased from
 * EventHandler, the file descriptor will be delete from epoll event automatically. So the
 * removeEvent function just decreased the file number.
 * @param fd : the file descriptor to remove
 * @return
 *      @retval 0 : remove success
 *      @retval other : remove error
 */
int Reactor::removeHandler(int fd) {
    int ret = 0;
    if(_handlers.end() != _handlers.find(fd)) {
        ret = _demultiplexer->removeEvent(fd);
        _handlers.erase(fd);
    }
    return ret;
}

/**
 * @brief Begin to dispatch events
 * @param timeout : timeout value
 */
void Reactor::dispatch(int timeout) {
    _demultiplexer->waitEvent(_handlers, timeout);
}



