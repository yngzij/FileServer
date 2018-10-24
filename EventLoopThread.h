//
// Created by Yang ziJ on 2018/10/22.
//

#ifndef FILESERVER_EVENTLOOPTHREAD_H
#define FILESERVER_EVENTLOOPTHREAD_H


#include <functional>
#include <string>
#include <mutex>
#include <thread>

namespace net {
    class EventLoop;
    class EventLoopThread {
    public:
        typedef std::function<void(EventLoop *)>ThreadInitCallback;
        EventLoopThread(const ThreadInitCallback &cb=ThreadInitCallback(),const std::string &name=std::string());
        ~EventLoopThread();
        EventLoop *startLoop();

    private:
        void threadFunc();
        EventLoop *loop_;
        std::shared_ptr<std::thread>thread_;
        bool exiting_;
        std::mutex mutex_;
        std::condition_variable cond_;
        ThreadInitCallback callback_;
    };
}

#endif //FILESERVER_EVENTLOOPTHREAD_H
