
#ifndef H_SYMPHONY_EVENT_HANDLER
#define H_SYMPHONY_EVENT_HANDLER

#include <map>
#include <vector>
#include "event.h"
#include "event-handler-base.h"
#include "event-handler-function.h"
#include "event-handler-method.h"
#include "typeinfo.h"

using std::map;
using std::vector;

class EventHandler {
  public:
    virtual ~EventHandler(void);
    void handle(Event* event);

    // to register a function...
    template <class EventType>
    void registerHandler(bool (*function)(EventType*)) {
      _handlers[Typeinfo(typeid(EventType))].push_back(new EventHandlerFunction<EventType>(function));
      return;
    }

    // to register a [non-static] member method...
    template <class ItemType,class EventType>
    void registerHandler(ItemType* item, bool (ItemType::*method)(EventType*)) {
      _handlers[Typeinfo(typeid(EventType))].push_back(new EventHandlerMethod<ItemType,EventType>(item, method));
      return;
    }

  private:
    map<Typeinfo,vector<EventHandlerBase*> > _handlers;
};

#endif // #ifndef H_SYMPHONY_EVENT_HANDLER
