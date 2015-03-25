﻿
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

void Job::setup(void* who_) {
  _counter = nextIndex();
  _who = who_;
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
  }
  return;
}

void RecurringJob::calculateNextTime(void) {
  time_t now = time(NULL);
  _when = now + (_upper > _lower ? Math::rand(_lower, _upper) : _lower);
  return;
}
