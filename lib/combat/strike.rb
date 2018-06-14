module Combat
  class Strike
    include ::DoesDamage

    attr_reader :attacker, :defender # combatants (Beings)
    attr_reader :parent, :bout, :battle # references
    attr_reader :level # nesting level of this Strike within the Bout
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
      @level      = if @parent.nil? then 0 else @parent.level + 2 end
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

      attacker.indented_puts level, 'Your $strike $string $being! [$string]', self, verb, defender, dam
      defender.indented_puts level, "$being's $strike $string you! [$string]", attacker, self, verb, dam
      attacker.room.indented_puts level, '$being $strike $string $being!', attacker, self, verb, defender, exclude: [attacker, defender]

      attacker.stamina.decrease 1
      defender.take_damage dam, self
    end

    ###########################################################################
    def whodunnit
      attacker
    end

    ###########################################################################
    def action
      @action ||= case created_by
                  when :second_attack then "second strike"
                  when :third_attack then "third strike"
                  when :fourth_attack then "fourth strike"
                  when :riposte then "riposte"
                  when :kick then "kick"
                  else "strike"
                  end

      @action
    end

    ###########################################################################
    def avoid
      @avoided = true
    end

    ###########################################################################
    def before
      # During normal autocombat, a Bout will have been created and assigned
      # but for offensive skills that isn't the case.
      @bout.push self if @bout

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
