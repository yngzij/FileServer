//
// Created by Yang ziJ on 2018/10/22.
//

#define _GNU_SOURCE 1
#include <cassert>
#include <poll.h>
#include "Channel.h"

net::Channel::Channel(net::EventLooop *loop, int fd):fd_(fd),loop_(loop),revents_(0),index_(-1),logHup_(true),eventHandling_(
        false),addedToLoop_(false),tied_(false) {

}

net::Channel::~Channel() {
    assert(!eventHandling_);
    assert(!addedToLoop_);

}

void net::Channel::handleEvent(Timestamp receiveTime) {
    std::shared_ptr<void>guard;
    if(tied_){
        guard=tie_.lock();
        if(guard){
            handleEventWithGuard(receiveTime);
        }
    } else{
        handleEventWithGuard(receiveTime);
    }
}


void net::Channel::update() {
    addedToLoop_= true;
    loop_.updateChannel(this);
}

void net::Channel::handleEventWithGuard(Timestamp receiveTime) {
    eventHandling_=true;
    if(revents_&POLLHUP&&!(revents_&POLLIN)){
        if(logHup_){
            printf("Channel ::handle_evnet() POLLHUP");
        }
        if(closeCallback_)
            closeCallback_();
    }
    if(revents_&POLLNVAL){
        printf("Channel ::handle_event() POLLNVAL");
    }

    if(revents_&(POLLERR|POLLNVAL)){
        if(errorCallback_)
            errorCallback_();
    }

    if(revents_&(POLLIN|POLLPRI|POLLRDHUP)){
        if(readCallback_)
            readCallback_(receiveTime);
    }
    if(revents_&POLLOUT){
        if (writeCallback_)
            writeCallback_();
    }
    eventHandling_= false;

}


std::string net::Channel::revertsToString() const {
    return std::string();
}

void net::Channel::remove() {
    assert(isNoneEvent());
    addedToLoop_= false;
    loop_->removeChannel(this);
}

void net::Channel::tie(const std::shared_ptr<void>&obj){
    tie_=obj;
    tied_= true;
}


