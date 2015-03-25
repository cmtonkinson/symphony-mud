
#ifndef H_SYMPHONY_EVENT_HANDLER
#define H_SYMPHONY_EVENT_HANDLER

#include "event.hpp"

///////////////////////////////////////////////////////////////////////////////
// ADT BASE
///////////////////////////////////////////////////////////////////////////////
class EventHandlerBase {
  public:
    virtual ~EventHandlerBase(void)   { return; }

    bool call(Event* event)           { return _call(event); }
    virtual bool _call(Event* event)  = 0;
};

///////////////////////////////////////////////////////////////////////////////
// FUNCTION HANDLER
///////////////////////////////////////////////////////////////////////////////
template <class EventType>
class EventHandlerFunction: public EventHandlerBase {
  public:
    EventHandlerFunction(bool (*function)(EventType*)): _function(function) { return; }
    virtual ~EventHandlerFunction(void) { return; }

    virtual bool _call(Event* event) { return (*_function)(static_cast<EventType*>(event)); }

  private:
    bool (*_function)(EventType*);
};

///////////////////////////////////////////////////////////////////////////////
// METHOD HANDLER
///////////////////////////////////////////////////////////////////////////////
template <class ObjectType,class EventType>
class EventHandlerMethod: public EventHandlerBase {
  public:
    EventHandlerMethod(ObjectType* item, bool (ObjectType::*method)(EventType*)): _object(item), _method(method) { return; }
    virtual ~EventHandlerMethod(void) { return; }
    virtual bool _call(Event* event) { return (_object->*_method)(static_cast<EventType*>(event)); }

  private:
    ObjectType* _object;
    bool (ObjectType::*_method)(EventType*);
};

#endif // #ifndef H_SYMPHONY_EVENT_HANDLER
