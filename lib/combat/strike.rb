module Combat
  class Strike
    include ::DoesDamage

    attr_reader :attacker, :defender, :parent, :bout, :battle, :level, :avoided, :created_by # combatants (Beings) # references # nesting level of this Strike within the Bout # is this Strike going to be prevented in some way? (bool) # through what mechanism was this attack made?

    # Strike objects execute immediately upon construction and don't outlive
    # the Battle.
    def initialize(attacker, defender, bout = nil, parent = nil, created_by = :bout)
      # Setup
      @attacker   = attacker
      @defender   = defender
      @bout       = bout
      @parent     = parent
      @level      = @parent.nil? ? 0 : @parent.level + 2
      @avoided    = false
      @created_by = created_by

      # Execute
      return if defender.dead?

      before
      perform unless avoided
      after
    end

    def perform
      bonus = attacker.damage_bonus.current
      weapon = attacker.anatomy.primary_weapon
      weapon_bonus = attacker.level
      weapon_bonus = weapon.hitroll if weapon
      base_damage = (2 * attacker.level) + rand(attacker.level) + rand((bonus / 2)..bonus) + rand(weapon_bonus * 2)
      dam = Damage.new base_damage
      dam.source = attacker
      dam.send weapon.element_type.to_sym, weapon.element_ratio if weapon&.element_type
      dam.send weapon.damage_type.to_sym if weapon&.damage_type
      defender.calculate_damage dam

      attacker.indented_puts level, I18n.t('combat.strike.attacker'), self, dam, defender
      defender.indented_puts level, I18n.t('combat.strike.defender'), attacker, self, dam
      attacker.room.indented_puts level,
                                  I18n.t('combat.strike.room'),
                                  attacker,
                                  self,
                                  dam,
                                  defender,
                                  exclude: [attacker, defender]

      weapon&.trigger_effect :strike, self
      defender.take_damage dam
    end

    def whodunnit
      attacker
    end

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

    def avoid
      @avoided = true
    end

    def before
      # During normal autocombat, a Bout will have been created and assigned
      # but for offensive skills that isn't the case.
      @bout&.push self

      attacker.run_hook :before_strike_attack, self
      defender.run_hook :before_strike_defense, self
    end

    def after
      attacker.run_hook :after_strike_attack, self
      defender.run_hook :after_strike_defense, self
    end
  end
end
