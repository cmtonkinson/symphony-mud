module Util

  ###########################################################################
  # Returns the value if it is between min and max.
  # Returns min if the value is less than min.
  # Returns max if the value is greater than max.
  def self.bound(value, min, max)
    value < min ? (min) : (value > max ? max : value)
  end

end
