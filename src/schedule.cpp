
#include "schedule.h"

Schedule::Schedule(void) {
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
  return;
}

void Schedule::remove(Job* job) {
  _queue.erase(job);
  return;
}

bool Schedule::fire(void) {
  Job* j = NULL;
  if (!_queue.empty()) {
    j = *_queue.begin();
    if (j->ready()) {
      _queue.erase(_queue.begin());
      j->fire();
      if (j->isRecurring()) {
        j->recur(this);
      } else {
        delete j;
      }
      return true;
    }
  }
  return false;
}

long Schedule::size(void) const {
  return _queue.size();
}
