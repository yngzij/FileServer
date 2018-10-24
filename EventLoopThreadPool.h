//
// Created by Yang ziJ on 2018/10/22.
//

#ifndef FILESERVER_EVENTLOOPTHREADPOOL_H
#define FILESERVER_EVENTLOOPTHREADPOOL_H


#include <functional>
#include <string>
#include <vector>

namespace net {
    class EventLoop;
    class EventLoopThread;
    class EventLoopThreadPool {
    public:
        typedef std::function<void (EventLoop *)>ThreadInitCallback;
        EventLoopThreadPool();
        ~EventLoopThreadPool()= default;
        void init(EventLoop *base_loop,int number_threads);
        void start(const ThreadInitCallback &cb=ThreadInitCallback());

    private:
        EventLoop *base_Loop_;
        std::string name_;
        int num_threads_;
        std::vector<std::shared_ptr<EventLoopThread>>threads_;
        std::vector<EventLoop *>loops_;
        int next_;
        bool  started_;
    };
}

#endif //FILESERVER_EVENTLOOPTHREADPOOL_H
