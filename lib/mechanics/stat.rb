module Stat

  #############################################################################
  # Returns a gain for a stat based on a deterministic logistic function.
  #
  # level - The level of the Being (in the context of a level up, this is the NEW level).
  # base - The minimum gain for the growth function (i.e. the theoretical gain at level 1).
  # target - The maximum sum of all gains for a particular growth function from level 1 to 100.
  #
  # This growth function produces a deterministic gain for a given level based
  # on fixing two values - the initial value of the function (level = 0) and
  # the final accumulated total of values the curve will produce between levels
  # 1 and 100. In other words, we fix the initial value at x = 0 and also the
  # integral of the curve from 0 to 100, and work backwards on everything else.
  #
  # It's a method of reverse-engineering the gains given a desired stat value
  # at max-level. Base and target, respectively, are the factors used to tune
  # the function to achieve a given result.
  #
  # Example: Suppose a warrior should get a health boost of about 6 hp at level
  # 2, and a Hero warrior should have approximately 4k hp. This particular
  # warrior just reached level 19. The function call would look like
  #
  #     Util.logistic(19, 6, 4000)
  #     # =>
  #
  def logistic_gain(level, base, target)
    Util.logistic_curve level, limit, min, Realm::Being::HERO
  end

end
