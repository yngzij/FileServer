//
// Created by Yang ziJ on 2018/10/24.
//

#ifndef FILESERVER_INETADDRESS_H
#define FILESERVER_INETADDRESS_H

#include <cstdint>
#include <sys/socket.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <string>

namespace net {

    class InetAddress {
    public:
        explicit InetAddress(uint16_t port=0,bool loopbackonly= false);

        InetAddress(const std::string& ip, uint16_t port);

        InetAddress(const struct sockaddr_in& addr)
                : addr_(addr){}

        std::string toIp() const;
        std::string toIpPort() const;
        uint16_t toPort() const;

        const struct sockaddr_in& getSockAddrInet() const { return addr_; }
        void setSockAddrInet(const struct sockaddr_in& addr) { addr_ = addr; }

        uint32_t ipNetEndian() const { return addr_.sin_addr.s_addr; }
        uint16_t portNetEndian() const { return addr_.sin_port; }


        static bool resolve(const std::string& hostname, InetAddress* result);

    private:
        struct sockaddr_in addr_;

    };
}

#endif //FILESERVER_INETADDRESS_H
