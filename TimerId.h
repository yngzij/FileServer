//
// Created by Yang ziJ on 2018/10/22.
//

#ifndef FILESERVER_TIMERID_H
#define FILESERVER_TIMERID_H

#include <cstdint>

namespace  net {

    class TimerId {
    public:
        TimerId():timer_(nullptr),sequence_(0){

        }
        TimerId(TimerId* timer,int64_t seq):timer_(timer),sequence_(seq){

        }
        friend  class TimerQueue;

    private:
        TimerId *timer_;
        int64_t sequence_;
    };
}
#endif //FILESERVER_TIMERID_H
