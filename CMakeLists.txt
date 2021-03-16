cmake_minimum_required(VERSION 3.17)
project(Reactor)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
message(OpenCV_LIBS)
include_directories(OpenCV_INCLUDE_DIRS)


add_executable(Reactor  main.cpp
                        Network/ConnectionHandler.cpp
                        Network/ConnectionHandler.h
                        Network/ListenHandler.cpp
                        Network/ListenHandler.h
                        Network/Socket.cpp
                        Network/Socket.h
                        Network/Definations.h
                        Reactor/EpollDemultiplexer.cpp
                        Reactor/EpollDemultiplexer.h
                        Reactor/EventDemultiplexer.h
                        Reactor/Reactor.cpp
                        Reactor/Reactor.h
                        Utils/Buffer.cpp
                        Utils/Buffer.h
                        Utils/ThreadPool.cpp
                        Utils/ThreadPool.h)

target_link_libraries(Reactor pthread)
target_link_libraries(Reactor ${OpenCV_LIBS})