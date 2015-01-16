/*
 * $Id: event-handler-method.h 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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
