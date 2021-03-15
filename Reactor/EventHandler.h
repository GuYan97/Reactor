//
// Created by guyan on 2020/12/10.
//

#ifndef RSVP_EVENTHANDLER_H
#define RSVP_EVENTHANDLER_H

namespace rsvp {

/**
 * @brief Interface of event handler
 */
class EventHandler {
public:
    EventHandler() = default;
    virtual ~EventHandler() = default;
    virtual int getFd() = 0;
    virtual void handleRead() = 0;
    virtual void handleWrite() = 0;
    virtual void handleClose() = 0;
    virtual void handleError() = 0;
};
}



#endif //RSVP_EVENTHANDLER_H
