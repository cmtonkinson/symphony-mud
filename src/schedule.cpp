
#include <string.h>
#include "event-handler.h"
#include "schedule.h"

Schedule::Schedule(void) {
  memset(_job_history, 0, sizeof(double) * JOB_TIME_HISTORY);
  _job_history_index = 0;
  _job_history_filled = false;
  return;
}

Schedule::~Schedule(void) {
  while (!_queue.empty()) {
    delete *_queue.begin();
    _queue.erase(_queue.begin());
  }
  return;
}

void Schedule::add(Job* job) {
  _queue.insert(job);

  // If a "who" is set for the Job, insert it into the index set, keyed by the "who" pointer.
  if (job->who()) {
    _index[job->who()].insert(job);
  }
  return;
}

void Schedule::remove(Job* job) {
  _queue.erase(job);

  // If a "who" is set for the Job, remove the Job from that who's set in the index.
  if (job->who()) {
    // Remove this Job from the index set.
    _index[job->who()].erase(job);
    // Remove the entire index entry set if it the set is empty.
    if (_index[job->who()].empty()) _index.erase(job->who());
  }
  return;
}

bool Schedule::fire(void) {
  Job* job = NULL;

  if (!_queue.empty()) {
    job = *_queue.begin();
    if (job->ready()) {
      _queue.erase(_queue.begin());
      profileJob(job);
      if (job->isRecurring()) job->recur(this);
      else delete job;
      return true;
    }
  }
  return false;
}

long Schedule::size(void) const {
  return _queue.size();
}

// Though _index is routinely manipulated in add() and remove(), cleanup() is the primary logic for
// which the _index exists. cleanup() is meant as a callback to be made in the destructor of classes
// which are passed as the ItemType pointers in Job constructors.
void Schedule::cleanup(void* item) {
  if (_index.count(item) > 0) {
    for (auto iter : _index[item]) {
      remove(iter);
      delete iter;
    }
  }
  return;
}

void Schedule::profileJob(Job* job) {
  clock_t clock_ticks = 0;
  double milliseconds = 0.0;

  // Time Job::fire() in clock ticks.
  clock_ticks = clock();
  job->fire();
  clock_ticks = clock() - clock_ticks;

  // How much wall-time was that?
  milliseconds = ((double)clock_ticks) / CLOCKS_PER_MICROSEC;

  // Record the time in the circular array.
  _job_history[_job_history_index] = milliseconds;
  if (++_job_history_index >= JOB_TIME_HISTORY) {
    _job_history_index  = 0;
    _job_history_filled = true;
  }

  return;
}

double Schedule::meanJobTime(void) const {
  unsigned end = _job_history_filled ? _job_history_index : JOB_TIME_HISTORY;
  double mean  = _job_history[0];
  for (unsigned x = 1; x < end; ++x) mean += _job_history[x];
  mean /= end;
  return mean;
}

double Schedule::minJobTime(void) const {
  unsigned end = _job_history_filled ? _job_history_index : JOB_TIME_HISTORY;
  unsigned min = _job_history[0];
  for (unsigned x = 1; x < end; ++x) {
    if (_job_history[x] < min) {
      min = _job_history[x];
    }
  }
  return min;
}

double Schedule::maxJobTime(void) const {
  unsigned end = _job_history_filled ? _job_history_index : JOB_TIME_HISTORY;
  unsigned max = _job_history[0];
  for (unsigned x = 1; x < end; ++x) {
    if (_job_history[x] > max) {
      max = _job_history[x];
    }
  }
  return max;
}
