module Talent
  class HealSpell < Spell 

    def initialize(*args)
      super *args
      @category       = :spell
      @name           = :heal
      @spell_category = :cure
      @default_target = :self
    end

    def _added_to(being)
    end

    def _invoke(target)
      health = rand(able_being.level) * 3

      able_being.puts '$being looks comforted.', target
      target.puts 'You feel comforted.'
      able_being.room.puts '$being looks comforted.', target, exclude: [able_being, target]

      target.health.increase health
    end

  end
end
