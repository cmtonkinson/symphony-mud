
#ifndef H_SYMPHONY_JOB
#define H_SYMPHONY_JOB

#include <ctime>
#include "event-handler.hpp"
#include "event.hpp"

class Schedule;

///////////////////////////////////////////////////////////////////////////////
// Job
///////////////////////////////////////////////////////////////////////////////
class Job: public Event {
  public:

    Job(time_t when, EventHandlerBase* what): _when(when), _what(what) { setup(); }

    template <class EventType>
    Job(time_t when, bool (*function)(EventType*)): _when(when), _what(new EventHandlerFunction<EventType>(function)) { setup(); }

    // WARNING: Whenever this constructor is used, the _who pointer will be set. That triggers
    // accounting logic in the Schedule class, and requires that your ItemType destructor clean up
    // after itself so that Jobs for destroyed items are cleared from the Schedule.
    // Your ItemType destructor must call `Schedule::cleanup()` and pass itself as the argument.
    template <class ItemType,class EventType>
    Job(time_t when, ItemType* item, bool (ItemType::*method)(EventType*)): _when(when), _what(new EventHandlerMethod<ItemType,EventType>(item, method)) { setup(item); }

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
    // Even though the pointer is called "who" it represents any ItemType as specified in the
    // constructors above (be they Zones, Beings, Items, whatever).
    void*             _who;

  private:
    unsigned          _counter;
};

class JobComp {
  public:
    bool operator() (Job* lhs, Job* rhs) const;
};

///////////////////////////////////////////////////////////////////////////////
// RecurringJob
///////////////////////////////////////////////////////////////////////////////

// _lower must be set to a non-negative integer.  _upper is optional.  If
// _upper is greater than _lower, then the value of _when will be set randomly
// between _lower and _upper seconds in the future.  If _upper is not given
// (or is less than _lower), then _when will be set to exactly _lower seconds
// in the future. _togo specifies how many more times the job should be run. If
// _togo is negative, the job will recur indefinitely.
//
// Examples:
//   if _lower == 10 and _upper == 0, _when will be exactly 10 seconds into the future.
//
//   if _lower == 10 and _upper == 20, _when will be between 10 and 20 seconds
//   into the future.

class RecurringJob: public Job {
  public:
    RecurringJob(EventHandlerBase* what, time_t lower, time_t upper = 0, long togo = -1): Job(0, what), _lower(lower), _upper(upper), _togo(togo) { calculateNextTime(); }
    template <class EventType>
    RecurringJob(bool (*function)(EventType*), time_t lower, time_t upper = 0, long togo = -1): Job(0, function), _lower(lower), _upper(upper), _togo(togo) { calculateNextTime(); }
    template <class ItemType,class EventType>
    RecurringJob(ItemType* item, bool (ItemType::*method)(EventType*), time_t lower, time_t upper = 0, long togo = -1): Job(0, item, method), _lower(lower), _upper(upper), _togo(togo) { calculateNextTime(); }
    virtual ~RecurringJob(void);

    virtual bool  isRecurring(void) const     { return true; }
    virtual void  recur(Schedule* schedule);
    void          calculateNextTime(void);

  private:
    time_t  _lower;
    time_t  _upper;
    long    _togo;
};

#endif // #ifndef H_SYMPHONY_JOB
