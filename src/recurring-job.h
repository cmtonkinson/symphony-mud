/*
 * $Id: recurring-job.h 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_RECURRING_JOB
#define H_SYMPHONY_RECURRING_JOB

#include "job.h"

/*
 * _lower must be set to a non-negative integer.  _upper is optional.  If
 * _upper is greater than _lower, then the value of _when will be set randomly
 * between _lower and _upper seconds in the future.  If _upper is not given
 * (or is less than _lower), then _when will be set to exactly _lower seconds
 * in the future. _togo specifies how many more times the job should be run. If
 * _togo is negative, the job will recur indefinitely.
 *
 * Examples:
 *   if _lower == 10 and _upper == 0, _when will be exactly 10 seconds into the future.
 *
 *   if _lower == 10 and _upper == 20, _when will be between 10 and 20 seconds
 *   into the future.
 */

class RecurringJob: public Job {
  public:
    RecurringJob( EventHandlerBase* what, time_t lower, time_t upper = 0, long togo = -1 ): Job( 0, what ), _lower( lower ), _upper( upper ), _togo( togo ) { calculateNextTime(); }
    template <class EventType>
    RecurringJob( bool (*function)( EventType* ), time_t lower, time_t upper = 0, long togo = -1 ): Job( 0, function ), _lower( lower ), _upper( upper ), _togo( togo ) { calculateNextTime(); }
    template <class ObjectType,class EventType>
    RecurringJob( ObjectType* object, bool (ObjectType::*method)( EventType* ), time_t lower, time_t upper = 0, long togo = -1 ): Job( 0, object, method ), _lower( lower ), _upper( upper ), _togo( togo ) { calculateNextTime(); }
    virtual ~RecurringJob( void );

    virtual bool  isRecurring( void ) const     { return true; }
    virtual void  recur( Schedule* schedule );
    void          calculateNextTime( void );

  private:
    time_t  _lower;
    time_t  _upper;
    long    _togo;
};

#endif // #ifndef H_SYMPHONY_RECURRING_JOB
