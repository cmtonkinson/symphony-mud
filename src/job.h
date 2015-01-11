/*
 * $Id: job.h 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_JOB
#define H_SYMPHONY_JOB

#include <ctime>
#include "event.h"
#include "event-handler-function.h"
#include "event-handler-method.h"

class Schedule;

class Job: public Event {
  public:
    Job(time_t when, EventHandlerBase* what): _when(when), _what(what) { _counter = nextIndex(); return; }
    template <class EventType>
    Job(time_t when, bool (*function)(EventType*)): _when(when), _what(new EventHandlerFunction<EventType>(function)) { _counter = nextIndex(); return; }
    template <class ObjectType,class EventType>
    Job(time_t when, ObjectType* object, bool (ObjectType::*method)(EventType*)): _when(when), _what(new EventHandlerMethod<ObjectType,EventType>(object, method)) { _counter = nextIndex(); return; }
    virtual ~Job(void);

    bool              operator < (const Job& ref) const { return _when < ref.when(); }
    time_t            when(void) const                  { return _when; }
    EventHandlerBase* what(void) const                  { return _what; }
    bool              ready(void) const                 { return _when <= time(NULL); }
    bool              fire(void);
    void              kill(void)                        { delete _what; _what = NULL; } // renders the job useless
    virtual bool      isRecurring(void) const           { return false; }
    virtual void      recur(Schedule* schedule)         { return; }
    unsigned          counter(void) const                 { return _counter; }

    static unsigned   nextIndex(void);

  protected:
    time_t            _when;

  private:
    EventHandlerBase* _what;
    unsigned          _counter;
};

class JobComp {
  public:
    bool operator() (Job* lhs, Job* rhs) const;
};

#endif // #ifndef H_SYMPHONY_JOB
