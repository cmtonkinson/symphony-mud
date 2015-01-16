
#ifndef H_SYMPHONY_SCHEDULE
#define H_SYMPHONY_SCHEDULE

#include <set>
#include "event-handler-method.h"
#include "job.h"

using std::multiset;

class Schedule {
  public:
    Schedule(void);
    virtual ~Schedule(void);

    void add(Job* job);
    void remove(Job* job);

    template <class EventType>
    void schedule(time_t when, bool (*function)(EventType*)) {
      if (function) {
        _queue.insert(new Job(when, new EventHandlerFunction<EventType>(function)));
      }
      return;
    }

    template <class ObjectType,class EventType>
    void schedule(time_t when, ObjectType* object, bool (ObjectType::*method)(EventType*)) {
      if (object && method) {
        _queue.insert(new Job(when, new EventHandlerMethod<ObjectType,EventType>(object, method)));
      }
      return;
    }

    bool  fire(void);
    long  size(void) const;

  private:
    multiset<Job*,JobComp> _queue;
};

#endif // #ifndef H_SYMPHONY_SCHEDULE
