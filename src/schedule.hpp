
#ifndef H_SYMPHONY_SCHEDULE
#define H_SYMPHONY_SCHEDULE

#include <map>
#include <set>
#include "job.hpp"

class Schedule {
  public:

    static const unsigned JOB_TIME_HISTORY    = 500;
    static const unsigned CLOCKS_PER_MICROSEC = CLOCKS_PER_SEC / 1000000;

    Schedule(void);
    virtual ~Schedule(void);

    void add(Job* job);
    void remove(Job* job);

    bool  fire(void);
    long  size(void) const;
    void  cleanup(void* item);

    void    profileJob(Job* job);
    double  meanJobTime(void) const;
    double  minJobTime(void) const;
    double  maxJobTime(void) const;

    const std::multiset<Job*,JobComp>&  queue() const { return _queue; }

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

    // Circular buffer to record the execution time of the last N Jobs fired.
    double    _job_history[JOB_TIME_HISTORY]; // circular array for job execution time in microseconds
    unsigned  _job_history_index;             // current "end" (one-past most recent entry) of the array
    bool      _job_history_filled;            // has the index wrapped around since initialization?

};

#endif // #ifndef H_SYMPHONY_SCHEDULE
