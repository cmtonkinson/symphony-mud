
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
