module Combat
  class Strike
    include ::DoesDamage

    attr_reader :attacker, :defender # combatants (Beings)
    attr_reader :parent, :bout, :battle # references
    attr_reader :avoided # is this Strike going to be prevented in some way? (bool)
    attr_reader :created_by # through what mechanism was this attack made?

    ###########################################################################
    # Strike objects execute immediately upon construction and don't outlive
    # the Battle.
    def initialize(attacker, defender, bout = nil, parent = nil, created_by = :bout)
      # Setup
      @attacker   = attacker
      @defender   = defender
      @bout       = bout
      @parent     = parent
      @avoided    = false
      @created_by = created_by

      # Execute
      return if defender.dead?
      before
      perform unless avoided
      after
    end

    ###########################################################################
    def perform 
      dam = 2 * attacker.level + rand(attacker.level)
      verb = Display::Format.damage dam

      attacker.puts 'Your strike $string $being! [$string]', verb, defender, dam
      defender.puts '$being strike $string you! [$string]', attacker, verb, dam
      attacker.room.puts '$being strike $string $being!', attacker, verb, defender, exclude: [attacker, defender]

      attacker.stamina.decrease 1
      defender.take_damage dam, self
    end

    ###########################################################################
    def whodunnit
      attacker
    end

    ###########################################################################
    def avoid
      @avoided = true
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
