
#ifndef H_SYMPHONY_EVENT_HANDLER_METHOD
#define H_SYMPHONY_EVENT_HANDLER_METHOD

#include "event.h"
#include "event-handler-base.h"

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

#endif // #ifndef H_SYMPHONY_EVENT_HANDLER_METHOD
