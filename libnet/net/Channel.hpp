#ifndef _CHANNEL_HPP
#define  _CHANNEL_HPP

#include "EventLoop.hpp"
#include "../utils/Timestamp.hpp"
#include <functional>

namespace net
{

/// This class doesn't own the fd.
/// The file descriptor could be a socketfd, an eventfd, a timerfd, or a signalfd.
class Channel
{
public:

  using EventCallback = std::function<void()> ;
  using ReadEventCallback = std::function<void(Timestamp)> ;

  Channel(EventLoop* loop, int fd);
  ~Channel();
  EventLoop* ownerLoop() {return loop_;}
  void handleEvent(Timestamp receiveTime);

  void removeSelf(){loop_->removeChannel(this);}
  
  void enableWriting(){events_ |= WRITE_EVENT; update();}
  void disableWriting(){events_ &= ~WRITE_EVENT; update();}
  void enableReading(){events_ |= READ_EVENT; update();}
  void disableReading(){events_ &= ~READ_EVENT; update();}
  void disableEvents() { events_ = NONE_EVENT; update(); }
  bool isNoneEvent() const {return events_ == NONE_EVENT;}

  void setReadCallback(ReadEventCallback&& cb){ readCallback_ = std::move(cb); }
  void setWriteCallback(EventCallback&& cb) {writeCallback_ = std::move(cb);}
  void setCloseCallback(EventCallback&& cb){closeCallback_ = std::move(cb); }
  void setErrorCallback(EventCallback&& cb) {errorCallback_ = std::move(cb);}

  int fd() const {return fd_; }
  int events() const {return events_;}
  void set_revents(int event){revents_ = event;}
  void set_index(int idx) {index_ = idx;}
  int index() const {return index_;}

private:

void update()
{
  loop_->updateChannel(this);
}


Channel(const Channel&) = delete;
Channel& operator=(const Channel&) = delete;

EventLoop* loop_;
int        fd_;
int        events_;
int        revents_; // it's the received event types of epoll or poll
int        index_;//used as arrayIndex(Poll) and state(Epoll)

ReadEventCallback readCallback_;
EventCallback writeCallback_;
EventCallback errorCallback_;
EventCallback closeCallback_;


static const int WRITE_EVENT;//benefit: to avoid include poll.h in this hpp
static const int READ_EVENT;
static const int NONE_EVENT;

};


}

#endif

