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
      attacker.puts "You strike $being!", defender
      defender.puts "$being strikes you!", attacker
      attacker.room.puts "$being strikes $being!", attacker, defender, exclude: [attacker, defender]

      attacker.stamina.decrease 1
      defender.take_damage 1 + rand(attacker.level), self
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

