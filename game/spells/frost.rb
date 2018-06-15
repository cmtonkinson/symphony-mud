module Talent
  class FrostSpell < Spell 

    def initialize(*args)
      super *args
      @category       = :spell
      @name           = :frost
      @spell_category = :harm
      @default_target = :enemy
    end

    def _added_to(being)
    end

    def _invoke(target)
      dam = rand(able_being.level / 2)
      verb = Display::Format.damage dam

      able_being.puts '$being shivers as frost $string $objective.', target, verb, target
      target.puts 'You shiver as frost $string you.', verb
      able_being.room.puts '$being shivers as frost $string you.', target, verb, exclude: [able_being, target]

      target.take_damage dam, able_being
    end

  end
end
