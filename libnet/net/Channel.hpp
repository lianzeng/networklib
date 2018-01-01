#ifndef _CHANNEL_HPP
#define  _CHANNEL_HPP

#include "EventLoop.hpp"
#include "Timestamp.hpp"
#include <functional>

namespace net
{

/// This class doesn't own the file descriptor.
/// The file descriptor could be a socket, an eventfd, a timerfd, or a signalfd.
class Channel
{
public:

  using EventCallback = std::function<void()> ;
  using ReadEventCallback = std::function<void(Timestamp)> ;

  Channel(EventLoop* loop, int fd);
  ~Channel();

  void handleEvent(Timestamp receiveTime);
 
  void enableWriting(){}

  void set_revents(int event){revents_ = event;}
  int fd() const {return fd_; }//debug

  void setReadCallback(ReadEventCallback&& cb){ readCallback_ = std::move(cb); }
  void setWriteCallback(EventCallback&& cb) {writeCallback_ = std::move(cb);}
  void setCloseCallback(EventCallback&& cb){closeCallback_ = std::move(cb); }
  void setErrorCallback(EventCallback&& cb) {errorCallback_ = std::move(cb);}

private:
Channel(const Channel&) = delete;
Channel& operator=(const Channel&) = delete;

EventLoop* loop_;
int        fd_;
int        events_;
int        revents_; // it's the received event types of epoll or poll

EventCallback writeCallback_;
EventCallback errorCallback_;
EventCallback closeCallback_;
ReadEventCallback readCallback_;



};


}

#endif

