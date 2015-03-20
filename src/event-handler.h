
#ifndef H_SYMPHONY_EVENT_HANDLER
#define H_SYMPHONY_EVENT_HANDLER

#include "event.h"

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
template <class ItemType,class EventType>
class EventHandlerMethod: public EventHandlerBase {
  public:
    EventHandlerMethod(ItemType* item, bool (ItemType::*method)(EventType*)): _item(item), _method(method) { return; }
    virtual ~EventHandlerMethod(void) { return; }
    virtual bool _call(Event* event) { return (_item->*_method)(static_cast<EventType*>(event)); }

  private:
    ItemType* _item;
    bool (ItemType::*_method)(EventType*);
};

#endif // #ifndef H_SYMPHONY_EVENT_HANDLER
