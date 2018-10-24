//
// Created by Yang ziJ on 2018/9/20.
//

#ifndef FILESERVER_SOCKETFD_H
#define FILESERVER_SOCKETFD_H
#include <cstdio>
#include "callback.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

namespace net {



    class SocketFd {
    public:
        explicit SocketFd(int fd);
        void setReusePort(bool on);
        void setReuseAddr(bool on);
        void setKeepAlive(bool on);
        ~SocketFd();
        int fd();

        void listen();

    private:
        const int sockfd_;
    };

    namespace setSockets{
        int createNonblockingOrDie();
        int connect(int sockfd,const struct sockaddr_in &addr);
        void bindOrDie(int sockfd,const struct sockaddr_in &addr);
        int accept(int sockfd,struct sockaddr_in *addr);
        ssize_t  read(int sockfd,void *buf,size_t count);
        ssize_t  write(int sockfd,const void *buf,size_t count);
        void close(int sockfd);
        void listenOrDie(int fd);
    }
};


#endif //FILESERVER_SOCKETFD_H
