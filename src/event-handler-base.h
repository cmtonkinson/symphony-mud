
#ifndef H_SYMPHONY_EVENT_HANDLER_BASE
#define H_SYMPHONY_EVENT_HANDLER_BASE

#include "event.h"

class EventHandlerBase {
  public:
    virtual ~EventHandlerBase(void)   { return; }

    bool call(Event* event)           { return _call(event); }
    virtual bool _call(Event* event)  = 0;
};

#endif // #ifndef H_SYMPHONY_EVENT_HANDLER_BASE
