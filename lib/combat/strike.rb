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
      bonus = attacker.damage_bonus.current
      weapon = attacker.anatomy.primary_weapon
      weapon_bonus = attacker.level
      weapon_bonus = weapon.hitroll if weapon
      dam = 2 * attacker.level + rand(attacker.level) + rand((bonus / 2)..bonus) + rand(weapon_bonus * 2)
      verb = Display::Format.damage dam

      attacker.indented_puts level, I18n.t('combat.strike.attacker'), self, verb, defender, dam
      defender.indented_puts level, I18n.t('combat.strike.defender'), attacker, self, verb, dam
      attacker.room.indented_puts level,
                                  I18n.t('combat.strike.room'),
                                  attacker,
                                  self,
                                  verb,
                                  defender,
                                  dam,
                                  exclude: [attacker, defender]

      attacker.stamina.decrease 1
      defender.take_damage dam, self
    end

    ###########################################################################
    def whodunnit
      attacker
    end

    ###########################################################################
    def action
      item = attacker.anatomy.primary_weapon
      verb = item ? item.damage_type : attacker.anatomy.default_damage

      @action ||= case created_by
                  when :second_attack then "second #{verb}"
                  when :third_attack then "third #{verb}"
                  when :fourth_attack then "fourth #{verb}"
                  when :riposte then "riposte #{verb}"
                  when :kick then 'kick'
                  else verb
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
