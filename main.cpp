#include <iostream>
#include "Utils/ThreadPool.h"
#include "Network/ConnectionHandler.h"
#include "Network/ListenHandler.h"
#include "Reactor/Reactor.h"
#include <sys/epoll.h>
#include <memory.h>

using namespace rsvp;

int main() {
    ThreadPool::getInstance().start(8);
    int listenfd = -1;
    if ((listenfd = socket(AF_INET, SOCK_STREAM  | SOCK_NONBLOCK, 0)) < 0) {
        std::cout<<"listen socket create fail"<<std::endl;
    }

    std::shared_ptr<ListenHandler> listenHandler = std::make_shared<ListenHandler>(listenfd);

    Reactor::getInstance().registerHandler(listenHandler, EPOLLIN | EPOLLOUT);

    while(true) {
        Reactor::getInstance().dispatch(-1);
    }
    return 0;
}
