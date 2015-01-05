/*
 * $Id: event-handler.cpp 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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

#include "event-handler.h"
#include "event-handler-method.h"

EventHandler::~EventHandler( void ) {
  map<Typeinfo,vector<EventHandlerBase*> >::iterator m_it;
  vector<EventHandlerBase*>::iterator v_it;
  for ( m_it = _handlers.begin(); m_it != _handlers.end(); ++m_it ) {
    for ( v_it = m_it->second.begin(); v_it != m_it->second.end(); ++v_it ) {
      delete *v_it;
    }
    m_it->second.clear();
  }
  _handlers.clear();
  return;
}

void EventHandler::handle( Event* event ) {
  map<Typeinfo,vector<EventHandlerBase*> >::const_iterator m_it = _handlers.find( Typeinfo(typeid(*event)) );
  vector<EventHandlerBase*>::const_iterator v_it;
  if ( m_it != _handlers.end() ) {
    for ( v_it = m_it->second.begin(); v_it != m_it->second.end(); ++v_it ) {
      (*v_it)->call( event );
    }
  }
  return;
}
