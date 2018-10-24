//
// Created by Yang ziJ on 2018/10/24.
//

#include <netdb.h>
#include <cassert>
#include "InetAddress.h"
#include "Sockets.h"
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include "endian.h"

#pragma GCC diagnostic ignored "-Wold-style-cast"
static const in_addr_t kInaddrAny = INADDR_ANY;
static const in_addr_t kInaddrLoopback = INADDR_LOOPBACK;

#pragma GCC diagnostic error "-Wold-style-cast"

net::InetAddress::InetAddress(uint16_t port, bool loopbackOnly) {
    bzero(&addr_, sizeof addr_);
    addr_.sin_family = AF_INET;
    in_addr_t ip = loopbackOnly ? kInaddrLoopback : kInaddrAny;
    addr_.sin_addr.s_addr = sockets::hostToNetwork32(ip);
    addr_.sin_port = sockets::hostToNetwork16(port);
}

net::InetAddress::InetAddress(const std::string &ip, uint16_t port) {
    bzero(&addr_, sizeof addr_);
    sockets::fromIpPort(ip.c_str(), port, &addr_);
}

std::string net::InetAddress::toIp() const {
    char buf[32];
    sockets::toIp(buf, sizeof buf, addr_);
    return buf;
}

std::string net::InetAddress::toIpPort() const {
    char buf[32];
    sockets::toIpPort(buf, sizeof buf, addr_);
    return buf;

}

uint16_t net::InetAddress::toPort() const {
    return sockets::networkToHost16(addr_.sin_port);

}
static __thread char t_resolveBuffer[64 * 1024];


bool net::InetAddress::resolve(const std::string &hostname, net::InetAddress *out) {
    assert(out != NULL);
    struct hostent hent;
    struct hostent* he = NULL;
    int herrno = 0;
    bzero(&hent, sizeof(hent));

    int ret = gethostbyname_r(hostname.c_str(), &hent, t_resolveBuffer, sizeof t_resolveBuffer, &he, &herrno);
    if (ret == 0 && he != NULL)
    {
        assert(he->h_addrtype == AF_INET && he->h_length == sizeof(uint32_t));
        out->addr_.sin_addr = *reinterpret_cast<struct in_addr*>(he->h_addr);
        return true;
    }
    else
    {
        if (ret)
        {
            std::cout << "InetAddress::resolve";
        }
        return false;
    }
}
