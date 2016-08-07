module Realm
  class Being

    # Core names can be grouped into five distinct buckets.
    EXPERIENCE = %i(level exp tnl)
    WELLNESS   = %i(health mana stamina)
    FITNESS    = %i(strength dexterity constitution intelligence focus creativity charisma luck)
    DEFENSE    = %i(armor bash slash pierce exotic)
    OFFENSE    = %i(hit_bonus damage_bonus)

    # Disallow direct manipulation of any core names.
    EXPERIENCE.each { |name| attr_reader name }
    WELLNESS.each { |name| attr_reader name }
    FITNESS.each { |name| attr_reader name }
    DEFENSE.each { |name| attr_reader name }
    OFFENSE.each { |name| attr_reader name }

    # Shortcuts!
    WELLNESS.each { |name| alias_method :"#{name[0..2]}", :"#{name}" }

    ###########################################################################
    # Set default values for performance attributes.
    after_initialize -> {
      @level = 1
      @exp   = 0
      @tnl   = 100
      WELLNESS.each { |name| instance_variable_set "@#{name}", Mechanics::MaxStat.new(100) }
      FITNESS.each { |name| instance_variable_set "@#{name}", Mechanics::ThresholdStat.new(15, 20) }
      DEFENSE.each { |name| instance_variable_set "@#{name}", Mechanics::SimpleStat.new(0) }
      OFFENSE.each { |name| instance_variable_set "@#{name}", Mechanics::SimpleStat.new(0) }
    }

    ###########################################################################
    on_hashify -> {
      hash = Hash.new

      EXPERIENCE.each { |name| hash[name] = send name }

      WELLNESS.each do |name|
        stat = send name
        hash[name] = "#{stat.current}/#{stat.max_stat.baseline}"
      end

      FITNESS.each do |name|
        stat = send name
        hash[name] = "#{stat.current_stat.baseline}:#{stat.threshold}"
      end

      DEFENSE.each { |name| hash[name] = send(name).baseline }
      OFFENSE.each { |name| hash[name] = send(name).baseline }

      {stats: hash}
    }

    ###########################################################################
    on_dehashify -> hash {
      hash = hash[:stats]

      EXPERIENCE.each { |name| instance_variable_set "@#{name}", hash[name] }

      WELLNESS.each do |name|
        cur, max = hash[name].split "/"
        instance_variable_set "@#{name}", Mechanics::MaxStat.new(max.to_i)
        send(name).current = cur.to_i
      end

      FITNESS.each do |name|
        baseline, threshold = hash[name].split ":"
        instance_variable_set "@#{name}", Mechanics::ThresholdStat.new(baseline.to_i, threshold.to_i)
      end

      DEFENSE.each { |name| instance_variable_set "@#{name}", Mechanics::SimpleStat.new(hash[name]) }
      OFFENSE.each { |name| instance_variable_set "@#{name}", Mechanics::SimpleStat.new(hash[name]) }
    }

  end
end
