
#ifndef H_SYMPHONY_SCHEDULE
#define H_SYMPHONY_SCHEDULE

#include <map>
#include <set>
#include "job.h"

class Schedule {
  public:
    Schedule(void);
    virtual ~Schedule(void);

    void add(Job* job);
    void remove(Job* job);

    bool  fire(void);
    long  size(void) const;
    void  cleanup(void* item);

  private:
    // The Schedule is composed of a multiset of Jobs, sorted primarily by the timestamp Job::_when
    // and secondarily by an incrementing counter (to avoid ambiguity with timers set for the same
    // time).
    std::multiset<Job*,JobComp>     _queue;
    // We need an infrastructure for efficiently removing Jobs from the Schedule when the "acting"
    // item (which is "this" in the Job execution) is destroyed, or else the Job will fire() and
    // best case results in a seg fault from accessing freed memory. For each item creating Jobs
    // we key a map with the items address, and maintain a set of outstanding Jobs for said item.
    std::map<void*,std::set<Job*>>  _index;
};

#endif // #ifndef H_SYMPHONY_SCHEDULE
