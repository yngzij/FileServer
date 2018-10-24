//
// Created by Yang ziJ on 2018/9/20.
//

#ifndef FILESERVER_SINGLETON_H
#define FILESERVER_SINGLETON_H

template <typename  T>

class Singleton {
public:
    static Singleton<T>  &Instance(){
        if (value_ == nullptr) {
            value_=new T *;
        }
        return *value_;
    };
private:

    static T *value_;
};


template <typename T>
T * Singleton<T>::value_= nullptr;

#endif //FILESERVER_SINGLETON_H
