//
// Created by Yang ziJ on 2018/9/20.
//

#ifndef FILESERVER_CALLBACK_H
#define FILESERVER_CALLBACK_H


#include <memory>
#include <functional>
#include "Timestamp.h"


template <typename From , typename To>
inline To implicit_cast(From const &f){
    return f;
}

template <typename To,typename From>
inline To down_cast(From *f){
    return static_cast<To>(f);
}

template <typename To ,typename From>
inline std::shared_ptr<To>down_pointer_cast(const std::shared_ptr<From>&f){
    return std::static_pointer_cast<To>(f);
}

namespace net{
    class Buffer;
    class TcpConnection;
    typedef std::shared_ptr<TcpConnection>TcpConnectionPtr;
    typedef std::function<void()>TimeCallback;
    typedef std::function<void(const  TcpConnectionPtr&)>ConnectionCallback;
    typedef std::function<void(const TcpConnectionPtr  &)>CloseCallback;
    typedef std::function<void(const TcpConnectionPtr &)>WriteCompleteCallback;
    typedef std::function<void (const TcpConnectionPtr &)>HighWaterMarkCallback;


    typedef std::function<void(const TcpConnectionPtr &,Buffer*,Timestamp)>MessageCallback;
    void defaultConnectionCallback(const TcpConnectionPtr &conn);
    void defaultMessageCallback(const TcpConnectionPtr &conn,Buffer* buffer,Timestamp receiveTime );

}

#endif //FILESERVER_CALLBACK_H
