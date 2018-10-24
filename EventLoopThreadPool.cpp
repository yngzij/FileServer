//
// Created by Yang ziJ on 2018/10/22.
//

#include <cassert>
#include "EventLoopThreadPool.h"
#include "EventLoop.h"
net::EventLoopThreadPool::EventLoopThreadPool():base_Loop_(nullptr),num_threads_(0),next_(0),started_(false) {

}

void net::EventLoopThreadPool::start(const net::EventLoopThreadPool::ThreadInitCallback &cb) {
    assert(base_Loop_);
    assert(!num_threads_);

    started_= true;

    for(int i=0;i<num_threads_;++i){
        char buf[name_.size()+32];
        snprintf(buf,sizeof buf,"%s%d",name_.c_str(),i);

        std::shared_ptr<EventLoopThread>t(new EventLoopThread(cb,buf));
        threads_.push_back(t);
        //loops_.push_back(t)
    }
    if(num_threads_==0&&cb){
        cb(base_Loop_);
    }
}


void net::EventLoopThreadPool::init(net::EventLoop *base_loop, int number_threads) {
    base_Loop_=base_loop;
    num_threads_=number_threads;
}



