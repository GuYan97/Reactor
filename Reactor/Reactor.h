//
// Created by guyan on 2020/12/10.
//

#ifndef RSVP_REACTOR_H
#define RSVP_REACTOR_H
#include "EventHandler.h"
#include "EventDemultiplexer.h"
#include <memory>
namespace rsvp {
/**
 * @brief Reactor, manage handlers and events
 */
class Reactor {
public:
    static Reactor& getInstance() {
        static Reactor reactor;
        return reactor;
    }

    ~Reactor();
    int registerHandler(const std::shared_ptr<EventHandler>& handler, int event);
    int removeHandler(const std::shared_ptr<EventHandler>& handler);
    int removeHandler(int fd);
    void dispatch(int timeout);
private:
    Reactor();
    std::unique_ptr<EventDemultiplexer> _demultiplexer;
    std::map<int, std::shared_ptr<EventHandler>> _handlers;
};

}


#endif //RSVP_REACTOR_H
