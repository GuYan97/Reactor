//
// Created by guyan on 2020/12/10.
//

#ifndef RSVP_EVENTDEMULTIPLEXER_H
#define RSVP_EVENTDEMULTIPLEXER_H
#include <map>
#include <memory>
#include "EventHandler.h"
namespace rsvp {

/**
 * @brief The interface of event demultiplexer
 */
class EventDemultiplexer {
public:
    EventDemultiplexer() = default;;
    virtual ~EventDemultiplexer() = default;;
    virtual int waitEvent(std::map<int, std::shared_ptr<EventHandler>>& handlers, int timeout) = 0;
    virtual int registerEvent(int fd, int event) = 0;
    virtual int removeEvent(int fd) = 0;
};
}

#endif //RSVP_EVENTDEMULTIPLEXER_H
