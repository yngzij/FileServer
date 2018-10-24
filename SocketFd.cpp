//
// Created by Yang ziJ on 2018/9/20.
//


#include "SocketFd.h"
#include <unistd.h>
#include <sys/types.h>
#include <cerrno>

using  namespace net;


void setNonBlockAndCloseOnExec(int sockfd){
    int flags=fcntl(sockfd,F_GETFL,0);
    flags|=O_NONBLOCK;
    int ret=fcntl(sockfd,F_SETFL,flags);
    flags=fcntl(sockfd,F_GETFD,0);
    flags|=FD_CLOEXEC;
    ret=fcntl(sockfd,F_SETFD,flags);
    (void )ret;
}

SocketFd::SocketFd(int fd):sockfd_(fd) {
}

SocketFd::~SocketFd() {
    close(sockfd_);
}

int SocketFd::fd() {
    return sockfd_;
}

void SocketFd::listen() {
    setSockets::listenOrDie(sockfd_);
}

void SocketFd::setReusePort(bool on) {
    int opt=on?1:0;
    setsockopt(sockfd_,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
}

void SocketFd::setReuseAddr(bool on) {
    int opt= on?1:0;
    int ret=setsockopt(sockfd_,SOL_SOCKET,SO_REUSEPORT,&opt,sizeof(&opt));
    if(ret<0&&on){
        printf("set so_reuseport error ");
    }
}

void SocketFd::setKeepAlive(bool on) {
    int opt=on?1:0;
    setsockopt(sockfd_,SOL_SOCKET,SO_KEEPALIVE,&opt,sizeof(&opt));
}


void setSockets::listenOrDie(int fd) {
    if(fd<0){
        printf("fd error ");
        return ;
    }

    listen(fd,100);
}




int setSockets::connect(int sockfd, const struct sockaddr_in &addr) {
    return connect(sockfd,(struct sockaddr *)&addr,sizeof(&addr));

}

void setSockets::bindOrDie(int sockfd, const struct sockaddr_in &addr) {

    int ret=bind(sockfd,(struct sockaddr *)&addr, sizeof(&addr));
    if(ret==-1){
        printf("bind sockfd error \n");
        return;
    }
}

int setSockets::accept(int sockfd, struct sockaddr_in *addr) {
    socklen_t  addrlen=sizeof *addr;

    int connfd=accept(sockfd,(sockaddr *)addr,&addrlen);
    setNonBlockAndCloseOnExec(sockfd);

    if(connfd<0){
        int saveError=errno;
        printf("accept error \n");
    }
    return sockfd;

}

ssize_t setSockets::read(int sockfd, void *buf, size_t count) {
    return  read(sockfd,buf,count);
}

ssize_t setSockets::write(int sockfd, const void *buf, size_t count) {
    return write(sockfd,buf,count);
}

void setSockets::close(int sockfd) {
    close(sockfd);
}

int setSockets::createNonblockingOrDie() {
    int sockfd=socket(AF_INET6,SOCK_STREAM,IPPROTO_TCP);
    if(sockfd<0){
        printf("sockets create failed \n");
        return -1;
    }
    return sockfd;
}



