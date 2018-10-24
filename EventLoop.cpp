//
// Created by Yang ziJ on 2018/10/22.
//

#include "EventLoop.h"
#include <iostream>
#include <sstream>
#include <cassert>
#include <zconf.h>
#include "EPollPoller.h"

#include "EventLoop.h"


__thread net::EventLoop * t_loopInThisThread=0;
const int kPollTimeMs=1;

    net::EventLoop::EventLoop():looping_(false),quit_(false),eventHandling_(false),callingPendingFunctors_(false),iteration_(0),threadId_(std::this_thread::get_id()),poller_(new EPollPoller(this),wakeupFd_(createEventfd()),wakeupChannel(new Channel(this,wakeupFd_)),currentActiveChannel_(
            nullptr)) {
        if(t_loopInThisThread){
            printf("Another eventloop exits in this thread");
        } else{
            t_loopInThisThread=this;
        }

        const auto eventLoop=bind(&EventLoop::handRead,this);
        wakeupChannel_->setReadCallback(eventLoop);
        wakeupChannel_->enableReading();
    }

    net::EventLoop::~EventLoop() {
        printf("Eventloog destructs in other thread");

        wakeupChannel_->disableAll();
        wakeupChannel_->remove();
        close(wakeupFd_);
        t_loopInThisThread= nullptr;
    }

    void net::EventLoop::loop() {
        assert(!looping_);
        assertInLoopThread();
        looping_ = true;
        quit_ = false;

        while (!quit_) {
            activeChannel_.clear();
            pollReturnTime_ = poller_->poll(kPollTimeMs,&activeChannel_);
            ++iteration_;

            eventHandling_= true;

            for(ChannelList::iterator it=activeChannel_.begin();it!=activeChannel_.end();++it){
                currentActiveChannel_=*it;
                currentActiveChannel_->handleEvent(pollReturnTime_);
            }

            currentActiveChannel_= nullptr;
            eventHandling_=false;
            doPendingFunctors();
            if(frameFunctor_){
                frameFunctor_();
            }
        }
    }

    void net::EventLoop::quit() {
        quit_=true;

        if(!isInLoopThread()){
            wakeup();
        }
    }

    void net::EventLoop::runInLoop(const net::EventLoop::Functor &cb) {
        if(isInLoopThread()){
            cb();
        } else{
            queueInLoop(cb);
        }
    }

    void net::EventLoop::queueInLoop(const net::EventLoop::Functor &cb) {
        std::unique_lock<std::mutex>lock(mutex_);
        pendingFunctors_.push_back(cb);

        if(isInLoopThread()||callingPendingFunctors_){
            wakeup();
        }
    }


    bool net::EventLoop::isInLoopThread() {
        return threadId_ == std::this_thread::get_id();
    }

    void net::EventLoop::abortNotLoopThread() {
        std::stringstream ss;

        ss << "Thread id_=" << std::this_thread::get_id() << '\n';
        std::cout << ss.str();
    }

    net::TimerId net::EventLoop::runAt(const Timestamp &time, net::TimeCallback &&cb) {
        return net::TimerId();
    }

    net::TimerId net::EventLoop::runAfter(double delay, net::TimeCallback &&cb) {
        return net::TimerId();
    }

    void net::EventLoop::hasChannel(net::Channel *channel) {

    }

    net::TimerId net::EventLoop::runEvery(double interval, net::TimeCallback &&cb) {
        return net::TimerId();
    }

void net::EventLoop::doPendingFunctors() {
    std::vector<Functor>functors;
    callingPendingFunctors_=true;

    {
        std::unique_lock<std::mutex>lock(mutex_);
        functors.swap(pendingFunctors_);
    }
    for(size_t t=0;i<functors.size();++i){
        functors[i]();
    }
    callingPendingFunctors_=false;

}

void net::EventLoop::setFrameFunctor(const net::EventLoop::Functor &cb) {
    frameFunctor_=cb;

}

void net::EventLoop::wakeup() {
    uint64_t one=1;
    ssize_t n=sockets::write(wakeupFd_,&one, sizeof(one));
    if(n!= sizeof(one)){
        printf("EventLoop::wakeup() writes %d bytes instead of 8",n);
    }

}

void net::EventLoop::updateChannel(net::Channel *channel) {

}

void net::EventLoop::removeChannel(net::Channel *channel) {

}

