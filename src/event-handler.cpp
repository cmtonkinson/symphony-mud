
#include "event-handler.h"
#include "event-handler-method.h"

EventHandler::~EventHandler(void) {
  map<Typeinfo,vector<EventHandlerBase*> >::iterator m_it;
  vector<EventHandlerBase*>::iterator v_it;
  for (m_it = _handlers.begin(); m_it != _handlers.end(); ++m_it) {
    for (v_it = m_it->second.begin(); v_it != m_it->second.end(); ++v_it) {
      delete *v_it;
    }
    m_it->second.clear();
  }
  _handlers.clear();
  return;
}

void EventHandler::handle(Event* event) {
  map<Typeinfo,vector<EventHandlerBase*> >::const_iterator m_it = _handlers.find(Typeinfo(typeid(*event)));
  vector<EventHandlerBase*>::const_iterator v_it;
  if (m_it != _handlers.end()) {
    for (v_it = m_it->second.begin(); v_it != m_it->second.end(); ++v_it) {
      (*v_it)->call(event);
    }
  }
  return;
}
