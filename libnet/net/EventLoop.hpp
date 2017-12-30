#ifndef _NET_EVENT_LOOP_HPP
#define _NET_EVENT_LOOP_HPP

namespace net
{

class EventLoop
{
public:
EventLoop();
~EventLoop();

void loop();

private:
  EventLoop(const EventLoop&) = delete;
  EventLoop& operator=(const EventLoop&) = delete;

};


}


#endif