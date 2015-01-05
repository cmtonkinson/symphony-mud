/*
 * $Id: schedule.cpp 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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

#include "schedule.h"

Schedule::Schedule( void ) {
  return;
}

Schedule::~Schedule( void ) {
  while ( !_queue.empty() ) {
    delete *_queue.begin();
    _queue.erase( _queue.begin() );
  }
  return;
}

void Schedule::add( Job* job ) {
  _queue.insert( job );
  return;
}

bool Schedule::fire( void ) {
  Job* j = NULL;
  if ( !_queue.empty() ) {
    j = *_queue.begin();
    if ( j->ready() ) {
      _queue.erase( _queue.begin() );
      j->fire();
      if ( j->isRecurring() ) {
        j->recur( this );
      } else {
        delete j;
      }
      return true;
    }
  }
  return false;
}

long Schedule::size( void ) const {
  return _queue.size();
}
