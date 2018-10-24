#include <memory>

//
// Created by Yang ziJ on 2018/10/22.
//

#include "EventLoopThread.h"
#include "EventLoop.h"

net::EventLoopThread::EventLoopThread(const net::EventLoopThread::ThreadInitCallback &cb, const std::string &name)
:loop_(nullptr),exiting_(false),callback_(cb) {

}

net::EventLoop *net::EventLoopThread::startLoop() {
    thread_ = std::make_shared<std::thread>(std::bind(&net::EventLoopThread::threadFunc,this));
}


void net::EventLoopThread::threadFunc() {
    EventLoop loop;
    if(callback_){
        callback_(&loop);
    }
    {
    std::unique_lock<std::mutex>lock(mutex_);
    loop_=&loop;
    cond_.notify_all();
    }
    loop_.loop();
}



