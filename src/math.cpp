
#include <cmath>
#include <cstdlib>
#include "math.h"

// Default state for Box-Muller variables.
bool Math::has_extra_random = false;
double Math::extra_random   = 0.0;

unsigned Math::rand(unsigned min, unsigned max) {
  return (::rand() % (max-min+1) + min);
}

// https://en.wikipedia.org/wiki/Logistic_function
double Math::logistic(double x, double limit, double min, double half_max_x) {
  double a = (limit - min) / min;
  double k = log(a) / half_max_x;
  double result = limit / (1 + a * pow(M_E, -k * x));
  return result;
}

// https://en.wikipedia.org/wiki/Box-Muller_transform
double Math::random_normal(double mean, double stddev) {
  double u1 = 0.0;
  double u2 = 0.0;
  double z0 = 0.0;
  double z1 = 0.0;
  double result = 0.0;
  // Have we already generate a set of randoms and not used the extra?
  if (Math::has_extra_random) {
    Math::has_extra_random = false;
    result = Math::extra_random;
  }
  // Generate uniformly distributed random numbers on [0, 1).
  else {
    u1 = (double)::rand() / (double)RAND_MAX;
    u2 = (double)::rand() / (double)RAND_MAX;
    // Derive two normally distributed numbers from u1 and u2.
    z0 = sqrt(-2 * log(u1)) * cos(2 * M_PI * u2);
    z1 = sqrt(-2 * log(u1)) * sin(2 * M_PI * u2);
    // Store the first result for next time.
    Math::has_extra_random = true;
    Math::extra_random     = z0;
    // Return the second result.
    result = z1;
  }
  // Box-Muller produces a standard normal deviate so we need to augment the result to yield a
  // general normal deviate adherent to the given mean and standand deviation.
  return result * stddev + mean;
}

template <typename Num>
Num Math::bound(const Num value, const Num min, const Num max) {
  if (value < min) return min;
  else if (value < max) return value;
  else return max;
}

bool Math::percent_chance(unsigned percent) {
  return rand(1, 100) <= percent;
}
