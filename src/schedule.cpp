
#include "event-handler.h"
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
