//
// Created by guyan on 2020/12/10.
//

#ifndef RSVP_EPOLLDEMULTIPLEXER_H
#define RSVP_EPOLLDEMULTIPLEXER_H
#include "EventDemultiplexer.h"
namespace rsvp {

/**
 * @brief An implement of EventDemultiplexer, use epoll
 */
class EpollDemultiplexer : public EventDemultiplexer{
public:
    EpollDemultiplexer();
    ~EpollDemultiplexer() override;
    int waitEvent(std::map<int, std::shared_ptr<EventHandler>>& handlers, int timeout) override;
    int registerEvent(int fd, int event) override;
    int removeEvent(int fd) override;
private:
    int _epollfd;
    int _fdMax;
};
}



#endif //RSVP_EPOLLDEMULTIPLEXER_H
