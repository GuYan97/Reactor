//
// Created by guyan on 2020/12/10.
//

#include "EpollDemultiplexer.h"
#include <unistd.h>
#include <sys/epoll.h>
#include "../Utils/ThreadPool.h"
#include "../Reactor/Reactor.h"
using namespace rsvp;

/**
 * @brief Constructor of EpollDemultiplexer
 */
EpollDemultiplexer::EpollDemultiplexer() :
        _epollfd(epoll_create(1024)),
        _fdMax(0) {
}

/**
 * @brief Destructor of EpollDemultiplexer
 */
EpollDemultiplexer::~EpollDemultiplexer() {
    close(_epollfd);
}

/**
 * @brief Wait for event from registered handlers
 * @param handlers : map of matching fds and EventHandlers
 * @param timeout : timeout value
 * @return available fd number
 */
int EpollDemultiplexer::waitEvent(std::map<int, std::shared_ptr<EventHandler>>& handlers, int timeout) {
    struct epoll_event events[_fdMax];
    int fdNum = epoll_wait(_epollfd, events, _fdMax, timeout);
    if(fdNum < 0) {
        perror("epoll wait error");
        return fdNum;
    }

    ThreadPool& t = ThreadPool::getInstance();

    for(auto i = 0; i < fdNum; i++) {
        int fd = events[i].data.fd;
        if(events[i].events & EPOLLERR) {
            if(handlers.find(fd) != handlers.end()) {
                t.pushTask([capture0 = handlers[fd]] { capture0->handleError(); });
            }
            else
                perror("no matching handler");
        }
        else if(events[i].events & EPOLLRDHUP) {
            if(handlers.find(fd) != handlers.end()) {
                t.pushTask([capture0 = handlers[fd]] { capture0->handleClose(); });
            }
            else
                perror("no matching handler");
        }
        else if(events[i].events & EPOLLIN) {
            if(handlers.find(fd) != handlers.end()) {
                t.pushTask([capture0 = handlers[fd]] { capture0->handleRead(); });
            }
            else
                perror("no matching handler");
        }
        else if(events[i].events & EPOLLOUT) {
            if(handlers.find(fd) != handlers.end()) {
                t.pushTask([capture0 = handlers[fd]] { capture0->handleWrite(); });
            }
            else
                perror("no matching handler");
        }
        else
            perror("undefined epoll event");
    }
    return fdNum;
}

/**
 * @brief Register a fd for epoll
 * @param fd : file descriptor
 * @param event : the type of event you want to register
 * @return
 *      @retval 0 : add success
 *      @retval other : epoll_ctl ret
 */
int EpollDemultiplexer::registerEvent(int fd, int event) {
    struct epoll_event ev{};
    ev.data.fd = fd;
    ev.events = event | EPOLLET;
    int ret = epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &ev);
    if(0 != ret) {
        perror("epoll add error");
        return ret;
    }

    _fdMax++;
    return 0;
}

/**
 * @brief Remove a fd for epoll
 * @param fd : file descriptor
 * @return
 *      @retval 0 : add success
 *      @retval other : epoll_ctl ret
 */
int EpollDemultiplexer::removeEvent(int fd) {
    struct epoll_event ev{};
    int ret = epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, &ev);

    if(0 != ret) {
        perror("epoll delete error");
        return ret;
    }
    _fdMax--;
    return 0;
}
