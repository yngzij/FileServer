//
// Created by Yang ziJ on 2018/10/24.
//
#include <iostream>
#include "EPollPoller.h"
#include <errno.h>
#include <unistd.h>
#include <poll.h>
#include <sys/epoll.h>
#include <cassert>

static_assert(EPOLLIN == POLLIN, "EPOLLIN == POLLIN");
static_assert(EPOLLPRI == POLLPRI, "EPOLLPRI == POLLPRI");
static_assert(EPOLLOUT == POLLOUT, "EPOLLOUT == POLLOUT");
static_assert(EPOLLRDHUP == POLLRDHUP,"EPOLLRDHUP == POLLRDHUP");
static_assert(EPOLLERR == POLLERR, "EPOLLERR == POLLERR");
static_assert(EPOLLHUP == POLLHUP, "EPOLLHUP == POLLHUP");


namespace
{
    const int kNew = -1;
    const int kAdded = 1;
    const int kDeleted = 2;
}


net::EPollPoller::EPollPoller(net::EventLoop *loop):epollfd_(::epoll_create(EPOLL_CLOSE)),events_(kInitEventListSize),ownerLoop_(_loop) {
    if(epollfd_<0){
        printf("Epollpoller ::epollpoller");
    }
}


bool net::EPollPoller::hasChannel(net::Channel *channel) const {
    assertInLoopThread();
    ChannelMap::const_iterator it=channels_.find(channel->fd());
    return it!=channels_.end()&&it->second==channel;
}

Timestamp net::EPollPoller::poll(int timeoutMs, net::EPollPoller::ChannelList *activeChannel) {
    int numEVents=::Epoll_wait(epollfd_,&*events_.begin(), static_cast<int>(events_.size()),timeoutMs);

    int saveError=errno;
    Timestamp now(Timestamp::now());

    if(numEVents>0){
        std::cout<<numEVents<<std::endl;
        fillActiveChannels(numEVents,activeChannel);
        if(static_cast<size_t >(numEVents)==events_.size()){
            events_.resize(events_.size()*2);
        }

    } else {
        if(saveError !=EINTR){
            errno=saveError;
            printf("epollpoller ::poll\n");
        }

    }
    return now;
}

void net::EPollPoller::updateChannel(net::Channel *channel) {
    assertInLoopThread();
    std::cout << "fd = " << channel->fd() << " events = " << channel->events();
    const int index = channel->index();
    if (index == kNew || index == kDeleted)
    {
        // a new one, add with EPOLL_CTL_ADD
        int fd = channel->fd();
        if (index == kNew)
        {
            assert(channels_.find(fd) == channels_.end());
            channels_[fd] = channel;
        }
        else // index == kDeleted
        {
            assert(channels_.find(fd) != channels_.end());
            assert(channels_[fd] == channel);
        }
        channel->set_index(kAdded);
        update(EPOLL_CTL_ADD, channel);
    }
    else
    {
        // update existing one with EPOLL_CTL_MOD/DEL
        int fd = channel->fd();
        assert(channels_.find(fd) != channels_.end());
        assert(channels_[fd] == channel);
        assert(index == kAdded);
        if (channel->isNoneEvent())
        {
            update(EPOLL_CTL_DEL, channel);
            channel->set_index(kDeleted);
        }
        else
        {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}

void net::EPollPoller::remove(net::Channel *channel) {

}

net::EPollPoller::~EPollPoller() {

}




