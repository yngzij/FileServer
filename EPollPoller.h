//
// Created by Yang ziJ on 2018/10/24.
//

#ifndef FILESERVER_EPOLLPOLLER_H
#define FILESERVER_EPOLLPOLLER_H

#include <map>
#include <vector>
#include "Channel.h"
#include "EventLoop.h"
#include "Timestamp.h"
namespace net {
    class EPollPoller {
    public:
        typedef std::vector<Channel *>ChannelList;
        EPollPoller(EventLoop* loop);
        virtual ~EPollPoller();

        virtual Timestamp poll(int timeoutMs,ChannelList * activeChannel);
        virtual void updateChannel(Channel *channel);
        virtual void remove(Channel *channel);


        virtual  bool hasChannel(Channel * channel)const;
        void assertInLoopThread()const{
            ownerLoop_->assertInLoopThread();
        }
    private:
        static const int kInitEventListSize=16;
        typedef std::vector<struct epoll_event>EventList;
        int epollfd_;
        EventList events_;
        typedef std::map<int,Channel *>ChannelMap;
        ChannelMap channels_;
        EventLoop * ownerLoop_;
    };
}

#endif //FILESERVER_EPOLLPOLLER_H
