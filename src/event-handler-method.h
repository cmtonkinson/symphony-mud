
#ifndef H_SYMPHONY_EVENT_HANDLER_METHOD
#define H_SYMPHONY_EVENT_HANDLER_METHOD

#include "event.h"
#include "event-handler-base.h"

template <class ObjectType,class EventType>
class EventHandlerMethod: public EventHandlerBase {
  public:
    EventHandlerMethod(ObjectType* object, bool (ObjectType::*method)(EventType*)): _object(object), _method(method) { return; }
    virtual ~EventHandlerMethod(void) { return; }
    virtual bool _call(Event* event) { return (_object->*_method)(static_cast<EventType*>(event)); }

  private:
    ObjectType* _object;
    bool (ObjectType::*_method)(EventType*);
};

#endif // #ifndef H_SYMPHONY_EVENT_HANDLER_METHOD
