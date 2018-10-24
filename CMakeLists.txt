cmake_minimum_required(VERSION 3.12)
project(FileServer)

set(CMAKE_CXX_STANDARD 11)

add_executable(FileServer main.cpp Singleton.h TcpConnect.cpp TcpConnect.h TcpAccept.cpp TcpAccept.h SocketFd.cpp SocketFd.h callback.cpp callback.h FileManager.cpp FileManager.h EventLoopThreadPool.cpp EventLoopThreadPool.h EventLoop.cpp EventLoop.h EventLoopThread.cpp EventLoopThread.h Channel.cpp Channel.h Timestamp.cpp Timestamp.h TimerId.cpp TimerId.h EPollPoller.cpp EPollPoller.h Sockets.cpp Sockets.h InetAddress.cpp InetAddress.h endian.cpp endian.h)