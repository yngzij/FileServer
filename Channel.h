//
// Created by Yang ziJ on 2018/10/22.
//

#ifndef FILESERVER_CHANNEL_H
#define FILESERVER_CHANNEL_H

#include <memory.h>
#include <functional>
#include "Timestamp.h"

namespace net{
    class EventLooop;
    class Channel {
    public:
        typedef std::function<void()>EventCallback;
        typedef std::function<void(Timestamp)>ReadEventCallback;
        Channel(EventLooop * loop,int fd);
        ~Channel();

        void handleEvent(Timestamp receiveTime);
        void setReadCallback(const ReadEventCallback &cb){
            readCallback_=cb;
        }
        void setWriteCallback(const EventCallback &cb){
            writeCallback_=cb;
        }
        void setCloseCallback(const EventCallback &cb){
            closeCallback_=cb;
        }
        void setErrorCallback(const EventCallback &cb){
            errorCallback_=cb;
        }
        void tie(const std::shared_ptr<void>&);

        int fd()const {
            return fd_;
        }
        int evnets()const {
            return events_;
        }

        void set_reverts(int revt){
            revents_=revt;
        }

        bool isNoneEvent()const {
            return events_==kNoneEvent;
        }

        void enableReading(){
            events_|=kReadEvent;update();
        }

        void disableReading(){
            events_&=~kReadEvent;update();
        }

        void enableWrite(){
            events_|=kWriteEvent;update();
        }
        void disableWrite(){
            events_&=~kWriteEvent;update();
        }
        void disableAll(){
            events_|=kNoneEvent;update();
        }

        bool isWriteing()const {
            return static_cast<bool>(events_ & kWriteEvent);
        }
        int index(){
            return index_;
        }

        void setIndex(int index){
            index_=index;
        }

        std::string revertsToString()const;

        void doNotLogHup(){
            logHup_= false;
        }

        EventLooop * ownerLoop(){
            return loop_;
        }
        void remove();


    private:
        void update();
        void handleEventWithGuard(Timestamp receiveTime);

        static const int  kNoneEvent;
        static const int  kReadEvent;
        static const int kWriteEvent;

        EventLooop *loop_;
        const int  fd_;
        int  events_;
        int  revents_;
        int index_;
        bool logHup_;

        std::weak_ptr<void> tie_;
        bool tied_;
        bool eventHandling_;
        bool addedToLoop_;
        ReadEventCallback readCallback_;
        EventCallback writeCallback_;
        EventCallback closeCallback_;
        EventCallback errorCallback_;
    };

}

#endif //FILESERVER_CHANNEL_H
