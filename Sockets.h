//
// Created by Yang ziJ on 2018/10/24.
//

#ifndef FILESERVER_SOCKETS_H
#define FILESERVER_SOCKETS_H

#include <arpa/inet.h>
#include <netinet/tcp.h>

struct tcp_info;

namespace net{
    class InetAddress;
    class Socket {
    public:
        explicit Socket(int sockfd):sockfd_(sockfd){};
        ~Socket();

        int fd()const {
            return sockfd_;
        }

        bool getTcpInfo(struct tcp_info *)const;
        bool getTcpInfoString(char *buf,int len )const;
        void bindAddress(const InetAddress &localaddr);
        void listen();
        void shutdownWrite();
        void setTcpNoDelay(bool on);
        void setReuseAddr(bool on);
        void setReusePort(bool on);
        void setKeepAlive(bool on);

    private:
        const int sockfd_;
    };

    namespace sockets{
        int connect(int osckfd,const struct sockaddr_in &addr);
        void bindOrDie(int sockfd,const struct sockaddr_in &addr);
        int accept(int sockfd,struct sockaddr_in &addr);
        ssize_t read(int sockfd,void *buf,size_t count);
        ssize_t readv(int sockfd,const struct iovec *iov,int iovcnt);
        ssize_t write(int sockfd,void *buf,size_t count);
        void close(int sockfd);
        void shutdownWrite(int sockfd);

        void toIpPort(char *buf,size_t size,const struct sockaddr_in &addr);
        void toIP(char *buf,size_t size,const struct sockaddr_in &addr);
        void fromIpPort(const char *ip,uint16_t port,struct sockaddr_in *addr);

        int getSocketError(int sockfd);

        const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);
        struct sockaddr* sockaddr_cast(struct sockaddr_in* addr);
        const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr);
        struct sockaddr_in* sockaddr_in_cast(struct sockaddr* addr);

        struct sockaddr_in getLocalAddr(int sockfd);
        struct sockaddr_in getPeerAddr(int sockfd);
        bool isSelfConnect(int sockfd);
    }
}

#endif //FILESERVER_SOCKETS_H
