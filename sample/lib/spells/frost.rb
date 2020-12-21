module Talent
  class FrostSpell < Spell
    def initialize(*args)
      super(*args)
      @category       = :spell
      @name           = :frost
      @spell_category = :harm
      @default_target = :enemy
    end

    def _learned_by(being); end

    def _learn; end

    def _invoke(target)
      dam = Combat::Damage.new rand(able_being.level / 2)
      dam.source = able_being
      dam.ice(100)
      target.calculate_damage dam

      able_being.puts '$being shivers as frost $damage $objective.',
                      target,
                      dam,
                      target
      target.puts 'You shiver as frost $damage you.', dam
      able_being.room.puts '$being shivers as frost $damage you.',
                           target,
                           dam,
                           exclude: [able_being, target]

      target.take_damage dam
      Logistics::Integrator.apply_effect :chill, target, able_being

      # NOTE: Put this logic somewhere that isn't stupid!
      # TODO: Maybe specify whether abilities are aggressive and centralize
      #       the logic for embattling?
      Combat.embattle target, able_being
    end
  end
end
