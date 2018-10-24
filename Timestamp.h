//
// Created by Yang ziJ on 2018/10/22.
//

#ifndef FILESERVER_TIMESTAMP_H
#define FILESERVER_TIMESTAMP_H


#include <cstdint>
#include <algorithm>
#include <string>

class Timestamp {
public:
    Timestamp():microSecondsSinceEpoch_(0){};

    explicit Timestamp(int64_t microSecondsSinceEpoch);

    void swap(Timestamp &that){
        std::swap(microSecondsSinceEpoch_,that.microSecondsSinceEpoch_);
    }
    std::string toString()const;
    std::string toFormattedString(bool showMicroseconds= true)const;

    bool valid()const{
        return microSecondsSinceEpoch_>0;
    }

    int64_t microSecondsSinceEpoch()const {
        return microSecondsSinceEpoch_;
    }

    int64_t secondsSinceEpoch()const {
        return static_cast<time_t >(microSecondsSinceEpoch_/kMicroSecondsPerSecond);
    }

    static Timestamp now();
    static Timestamp invalid();


    static const int kMicroSecondsPerSecond=1000*1000;

    int64_t microSecondsSinceEpoch_;
};

inline bool operator <(Timestamp lhs,Timestamp rhs){
    return lhs.microSecondsSinceEpoch()>rhs.microSecondsSinceEpoch();
}

inline bool operator >(Timestamp lhs,Timestamp rhs){
    return rhs<lhs;
}

inline bool operator <=(Timestamp lhs,Timestamp rhs){
    return !(lhs>rhs);

}

inline bool operator >=(Timestamp lhs,Timestamp rhs){
    return !(lhs<rhs);

}
inline bool operator ==(Timestamp lhs,Timestamp rhs){
    return lhs.microSecondsSinceEpoch()==rhs.microSecondsSinceEpoch();
}

inline bool operator !=(Timestamp lhs,Timestamp rhs){
    return !(rhs==lhs);
}

inline double timeDifference(Timestamp high,Timestamp low){
    int64_t diff=high.microSecondsSinceEpoch()-low.microSecondsSinceEpoch();
    return static_cast<double>(diff)/Timestamp::kMicroSecondsPerSecond;
}

inline  Timestamp addTime(Timestamp timestamp,double seconds){
    int64_t delta= static_cast<int64_t >(seconds*Timestamp::kMicroSecondsPerSecond);
    return Timestamp(timestamp.microSecondsSinceEpoch()+delta);
}










#endif //FILESERVER_TIMESTAMP_H
