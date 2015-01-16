/*
 * $Id: event-handler.h 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
 *
 * This file is part of the Symphony project <http://code.google.com/p/symphonymud/>
 * Copyright 2005-2010 Chris Tonkinson <cmtonkinson@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    template <class ObjectType,class EventType>
    void registerHandler(ObjectType* object, bool (ObjectType::*method)(EventType*)) {
      _handlers[Typeinfo(typeid(EventType))].push_back(new EventHandlerMethod<ObjectType,EventType>(object, method));
      return;
    }

  private:
    map<Typeinfo,vector<EventHandlerBase*> > _handlers;
};

#endif // #ifndef H_SYMPHONY_EVENT_HANDLER
