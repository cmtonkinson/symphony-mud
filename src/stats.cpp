
#include <cmath>
#include "stats.h"
#include "creature.h"
#include "math.h"

unsigned Stats::logistic_gain(unsigned level, unsigned base, unsigned target) {
  double limit      = floor(target/50);
  double half_max_x = floor(LEVEL_HERO / 2);
  double result     = Math::logistic(level, limit, base, half_max_x);
  return floor(result);
}

unsigned Stats::cone_randomization(unsigned value) {
  return Math::random_normal(value, value / Stats::CONE_FACTOR);
}
