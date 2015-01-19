
#ifndef H_SYMPHONY_JOB
#define H_SYMPHONY_JOB

#include <ctime>
#include "event.h"
#include "event-handler-function.h"
#include "event-handler-method.h"

class Schedule;

class Job: public Event {
  public:

    Job(time_t when, EventHandlerBase* what): _when(when), _what(what) { setup(); }

    template <class EventType>
    Job(time_t when, bool (*function)(EventType*)): _when(when), _what(new EventHandlerFunction<EventType>(function)) { setup(); }

    // WARNING: Whenever this constructor is used, the _who pointer will be set. That triggers
    // accounting logic in the Schedule class, and requires that your ObjectType destructor clean up
    // after itself so that Jobs for destroyed objects are cleared from the Schedule.
    // Your ObjectType destructor must call `Schedule::cleanup()` and pass itself as the argument.
    template <class ObjectType,class EventType>
    Job(time_t when, ObjectType* object, bool (ObjectType::*method)(EventType*)): _when(when), _what(new EventHandlerMethod<ObjectType,EventType>(object, method)) { setup(object); }

    virtual ~Job(void);

    void              who(void* who)                    { _who = who; }
    void*             who(void) const                   { return _who; }
    void              what(EventHandlerBase* what)      { _what = what; }
    EventHandlerBase* what(void) const                  { return _what; }
    void              when(time_t when)                 { _when = when; }
    time_t            when(void) const                  { return _when; }

    bool              operator < (const Job& ref) const { return _when < ref.when(); }

    void              setup(void* who_ = nullptr);
    bool              ready(void) const                 { return _when <= time(NULL); }
    bool              fire(void);
    void              kill(void)                        { delete _what; _what = NULL; } // renders the job useless
    virtual bool      isRecurring(void) const           { return false; }
    virtual void      recur(Schedule* schedule)         { return; }
    unsigned          counter(void) const               { return _counter; }

    static unsigned   nextIndex(void);

  protected:
    time_t            _when;
    EventHandlerBase* _what;
    // Even though the pointer is called "who" it represents any ObjectType as specified in the
    // constructors above (be they Areas, Creatures, Objects, whatever).
    void*             _who;

  private:
    unsigned          _counter;
};

class JobComp {
  public:
    bool operator() (Job* lhs, Job* rhs) const;
};

#endif // #ifndef H_SYMPHONY_JOB
