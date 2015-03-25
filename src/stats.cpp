
#include <cmath>
#include "being.hpp"
#include "math.hpp"
#include "stats.hpp"

unsigned Stats::logistic(unsigned level, unsigned min, unsigned target) {
  double limit      = floor(target/50);
  double half_max_x = floor(Being::HERO / 2);
  double result     = Math::logistic(level, limit, min, half_max_x);
  return 0.5 + result;
}

int Stats::polynomial(int level, int base, int target) {
  double rate = log(target - base) / log(Being::HERO - 1);
  return 0.5 + base + pow(level - 1, rate);
}

int Stats::cone_randomization(int value, int cone_factor) {
  double rand = Math::random_normal(value, value / cone_factor);
  return 0.5 + rand;
}
