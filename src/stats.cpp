
#include <cmath>
#include "stats.h"
#include "creature.h"
#include "math.h"

unsigned Stats::logistic(unsigned level, unsigned base, unsigned target) {
  double limit      = floor(target/50);
  double half_max_x = floor(LEVEL_HERO / 2);
  double result     = Math::logistic(level, limit, base, half_max_x);
  return 0.5 + result;
}

int Stats::polynomial(int level, int base, int target) {
  double rate = log(target - base) / log(LEVEL_HERO - 1);
  return 0.5 + base + pow(level - 1, rate);
}

int Stats::cone_randomization(int value) {
  double rand = Math::random_normal(value, value / Stats::CONE_FACTOR);
  return 0.5 + rand;
}
