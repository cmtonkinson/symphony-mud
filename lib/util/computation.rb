module Util
  class Computation
    # Ensure a value is no less than min and no greater than max.
    #
    # Returns the value if it is between min and max.
    # Returns min if the value is less than min.
    # Returns max if the value is greater than max.
    def self.bound(value, min, max)
      value.clamp(min, max)
    end

    # Plain and simple implementation of the Logistic Function.
    # See: https://en.wikipedia.org/wiki/Logistic_function
    def self.logistic_curve(x, limit, min, half_max_x)
      a = 1.0 * (limit - min) / min
      k = 1.0 * Math.log(a) / half_max_x

      limit / (1 + (a * (Math::E**(-k * x))))
    end

    # Calculate the value of a polynomial curve for the given value of x, where
    # the curve is reverse engineered by interpolation with the values of x_max,
    # y_min, and y_max. x_min is defined as 1.
    def self.polynomial_curve(x, x_max, y_min, y_max)
      exponent = Math.log(y_max - y_min) / Math.log(x_max - 1)

      (1.0 * y_min) + ((x - 1)**exponent)
    end

    # Determine whether a randomized event will occur "this time" given a
    # probability expressed as a percent chance from 0 to 100.
    def self.percent_chance(percent)
      Random.rand(1..100) <= percent
    end
  end
end
