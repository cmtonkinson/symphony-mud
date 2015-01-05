/*
 * $Id: recurring-job.cpp 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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

#include <cstdlib>
#include "recurring-job.h"
#include "schedule.h"

RecurringJob::~RecurringJob( void ) {
  return;
}

void RecurringJob::recur( Schedule* schedule ) {
  if ( --_togo != 0 ) {
    calculateNextTime();
    schedule->add( this );
  }
  return;
}

void RecurringJob::calculateNextTime( void ) {
  time_t now = time( NULL );
  if ( _upper > _lower ) {
    _when = now + ( rand() % (_upper-_lower) + _lower );
  } else {
    _when = now + _lower;
  }
  return;
}
