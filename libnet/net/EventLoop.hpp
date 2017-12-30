#ifndef _NET_EVENT_LOOP_HPP
#define _NET_EVENT_LOOP_HPP

namespace net
{

class EventLoop
{
public:
EventLoop();


private:
  EventLoop(const EventLoop&) = delete;
  EventLoop& operator=(const EventLoop&) = delete;

};


}


#endif