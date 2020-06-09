module Talent
  class KickSkill < Ability
    include ActiveAbility

    def initialize(*args)
      super(*args)
      @category = :skill
      @name     = :kick
    end

    def _learn; end

    def _invoke(target)
      Combat.embattle able_being, target

      if rand(1..100) > success_rate
        able_being.puts 'You try to kick $being, but miss.', target
        target.puts '$being tries to kick you, but misses.', able_being
        able_being.room.puts '$being tries to kick $being, but misses.',
                             able_being,
                             target,
                             exclude: [able_being, target]
        nil
      else
        Combat::Strike.new able_being, target, nil, nil, name
      end
    end
  end
end
