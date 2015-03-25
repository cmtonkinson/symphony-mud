
#include <cstdlib>
#include "job.hpp"
#include "math.hpp"
#include "schedule.hpp"

///////////////////////////////////////////////////////////////////////////////
// Job
///////////////////////////////////////////////////////////////////////////////
Job::~Job(void) {
  if (_what) delete _what;
  return;
}

void Job::setup(void* who) {
  _counter = nextIndex();
  _who = who;
  return;
}

void Job::updateCounter(void) {
  _counter = nextIndex();
  return;
}

bool Job::fire(void) {
  // we don't want to fire with a null pointer...
  return _what ? _what->call(this) : false;
}

/*
 * Job::< simply compares _when, but JobComp::() must also take the _counter of the Job under
 * consideration so that for example erasing a Job from a std::set or std::multiset won't remove
 * all other Jobs that happen to have the same _when value.
 */
bool JobComp::operator()(Job* left, Job* right) const {
  if (left->when() == right->when()) {
    return left->counter() < right->counter();
  } else {
    return left->when() < right->when();
  }
}

unsigned Job::nextIndex(void) {
  static unsigned _counter_index = 0;
  return ++_counter_index;
}

///////////////////////////////////////////////////////////////////////////////
// RecurringJob
///////////////////////////////////////////////////////////////////////////////
RecurringJob::~RecurringJob(void) {
  return;
}

void RecurringJob::recur(Schedule* schedule) {
  // If _togo is negative, recur the Job infinitely.
  // Otherwise, decrement _togo and recur if the result is positive.
  if (_togo < 0 || --_togo > 0) {
    calculateNextTime();
    updateCounter();
    schedule->add(this);
  // If we're not actually going to repeat the job, it must be deleted.
  } else {
    delete this;
  }
  return;
}

void RecurringJob::calculateNextTime(void) {
  time_t now = time(NULL);
  _when = now + (_upper > _lower ? Math::rand(_lower, _upper) : _lower);
  return;
}
