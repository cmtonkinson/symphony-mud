
#include <cstdlib>
#include "recurring-job.h"
#include "schedule.h"

RecurringJob::~RecurringJob(void) {
  return;
}

void RecurringJob::recur(Schedule* schedule) {
  if (--_togo != 0) {
    calculateNextTime();
    schedule->add(this);
  }
  return;
}

void RecurringJob::calculateNextTime(void) {
  time_t now = time(NULL);
  if (_upper > _lower) {
    _when = now + (rand() % (_upper-_lower) + _lower);
  } else {
    _when = now + _lower;
  }
  return;
}
