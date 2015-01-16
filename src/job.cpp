
#include <cstdlib>
#include "job.h"
#include "schedule.h"

Job::~Job(void) {
  if (_what) {
    delete _what;
  }
  return;
}

bool Job::fire(void) {
  // we don't want to fire with a null pointer...
  return _what ? _what->call(this) : false;
}

/*
 * Job::< simply compares _when, but JobComp must also take the address of the Job under
 * consideration so that for example erasing a Job from a std::set won't remove all other Jobs that
 * happen to have the same _when value.
 */
bool JobComp::operator()(Job* left, Job* right) const {
  if (*left < *right) {
    return true;
  } else {
    return left->counter() < right->counter();
  }
}

unsigned Job::nextIndex(void) {
  static unsigned _counter_index = 0;
  return ++_counter_index;
}
