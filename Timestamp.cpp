//
// Created by Yang ziJ on 2018/10/22.
//

#include <chrono>
#include "Timestamp.h"
#include <chrono>


std::string Timestamp::toString() const {
    return std::string();
}

std::string Timestamp::toFormattedString(bool showMicroseconds) const {
    time_t seconds= static_cast<time_t >(microSecondsSinceEpoch_/kMicroSecondsPerSecond);
    struct tm tm_time;
    struct tm *ptm;
    ptm=localtime(&seconds);
    tm_time=*ptm;

    char buf[32]={0};
    if(showMicroseconds){
        int microseconds= static_cast<int>(microSecondsSinceEpoch_&kMicroSecondsPerSecond);
        snprintf(buf,sizeof buf ,"%4d%02d%02d %02d:%02d:%02d.%06d",tm_time.tm_year+1900,tm_time.tm_mon+1,tm_time.tm_mday,tm_time.tm_hour,tm_time.tm_min,tm_time.tm_sec,microseconds);
    } else{
        snprintf(buf,sizeof buf ,"%4d%02d%02d %02d:%02d:%02d",tm_time.tm_year+1900,tm_time.tm_mon+1,tm_time.tm_mday,tm_time.tm_hour,tm_time.tm_min,tm_time.tm_sec);
    }
    return buf;
}

Timestamp::Timestamp(int64_t microSecondsSinceEpoch) {

}

Timestamp Timestamp::now() {
    std::chrono::time_point<std::chrono::system_clock,std::chrono::microseconds>now=std::chrono
            ::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
    int64_t microSeconds=now.time_since_epoch().count();
    Timestamp time(microSeconds);
    return  time;
}

Timestamp Timestamp::invalid() {
    return {};
}
