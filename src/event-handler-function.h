
#ifndef H_SYMPHONY_EVENT_HANDLER_FUNCTION
#define H_SYMPHONY_EVENT_HANDLER_FUNCTION

#include "event.h"
#include "event-handler-base.h"

template <class EventType>
class EventHandlerFunction: public EventHandlerBase {
  public:
    EventHandlerFunction(bool (*function)(EventType*)): _function(function) { return; }
    virtual ~EventHandlerFunction(void) { return; }

    virtual bool _call(Event* event) { return (*_function)(static_cast<EventType*>(event)); }

  private:
    bool (*_function)(EventType*);
};

#endif // #ifndef H_SYMPHONY_EVENT_HANDLER_FUNCTION
