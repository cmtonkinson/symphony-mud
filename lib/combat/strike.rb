module Combat

  class Strike
    include ::DoesDamage

    attr_reader :attacker, :defender
    attr_reader :bout, :battle
    attr_reader :parent

    ###########################################################################
    def initialize(attacker, defender, bout = nil, parent = nil)
      @attacker = attacker
      @defender = defender
      @bout     = bout
      @parent   = parent

      before
      exec
      after
    end

    ###########################################################################
    def exec
      dam = attacker.level + rand(attacker.level)
      verb = Display::Format.damage dam

      attacker.puts "Your strike $string $being! [$string]", verb, defender, dam
      defender.puts "$being strike $string you! [$string]", attacker, verb, dam
      attacker.room.puts "$being strike $string $being!", attacker, verb, defender, exclude: [attacker, defender]

      attacker.stamina.decrease 1
      defender.take_damage dam, self
    end

    ###########################################################################
    def whodunnit
      attacker
    end

    ###########################################################################
    def before
      @bout.push self
      attacker.run_hook :before_strike_attack, self
      defender.run_hook :before_strike_defense, self
    end

    ###########################################################################
    def after
      attacker.run_hook :after_strike_attack, self
      defender.run_hook :after_strike_defense, self
    end

  end

end

