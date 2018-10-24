//
// Created by Yang ziJ on 2018/10/22.
//

#ifndef FILESERVER_EVENTLOOP_H
#define FILESERVER_EVENTLOOP_H

#include <functional>
#include <thread>
#include <vector>
#include "Timestamp.h"
#include "Timestamp.h"
#include "callback.h"
#include "Channel.h"
#include "TimerId.h"


namespace net {
    class EPollPoller;
    class TimerQueue;
    class Channel;
    class EventLoop {
    public:
        typedef std::function<void()> Functor;

        EventLoop();

        ~EventLoop();

        void loop();

        void quit();

        Timestamp pollReturnTime() const {
            return  pollReturnTime_;
        }

        int64_t iteration() const {
            return iteration_;
        }

        void runInLoop(const Functor &cb);

        void queueInLoop(const Functor &cb);

        TimerId runAt(const Timestamp &time,TimeCallback &&cb);
        TimerId runAfter(double delay,TimeCallback &&cb);
        TimerId runEvery(double interval,TimeCallback &&cb);

        void setFrameFunctor(const Functor &cb);



        void wakeup();
        void updateChannel(Channel *channel);
        void removeChannel(Channel *channel);
        void hasChannel(Channel *channel);


        bool isInLoopThread();

        void abortNotLoopThread();

        void assertInLoopThread(){
            if(!isInLoopThread()){
                abortNotLoopThread();
            }
        }


    private:
        typedef std::vector<Channel *>ChannelList;
        bool  looping_;
        bool quit_;
        bool eventHandling_;
        bool callingPendingFunctors_;
        int64_t  iteration_;
        const std::thread::id threadId_;
        Timestamp pollReturnTime_;
        std::shared_ptr<EPollPoller> poller_;
        std::shared_ptr<TimerQueue> timeQueue_;

        int wakeupFd_;
        std::shared_ptr<Channel> wakeupChannel_;
        ChannelList activeChannel_;
        Channel * currentActiveChannel_;
        std::mutex  mutex_;
        std::vector<Functor> pendingFunctors_;
        Functor  frameFunctor_;

        void doPendingFunctors();
    };
}

#endif //FILESERVER_EVENTLOOP_H
