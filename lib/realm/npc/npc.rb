module Realm
  class NPC < Being
    # NPCs (during building) have an obvious need for full stat manipulation
    # even though Being says that's 'discouraged'.
    EXPERIENCE.each { |name| attr_writer name }
    WELLNESS.each { |name| attr_writer name }
    FITNESS.each { |name| attr_writer name }
    DEFENSE.each { |name| attr_writer name }
    OFFENSE.each { |name| attr_writer name }

    SETTINGS = %i[mobility hostility]

    SETTINGS.each { |s| attr_reader s }

    ###########################################################################
    def initialize(id = nil, props = {})
      super
      SETTINGS.each { |s| instance_variable_set "@#{s}", 0 }
    end

    ###########################################################################
    on_hashify lambda {
      hash = {}
      SETTINGS.each { |s| hash[s] = send s }
      { npc: hash }
    }

    ###########################################################################
    on_dehashify lambda { |hash|
      npc = hash[:npc]
      SETTINGS.each { |s| instance_variable_set "@#{s}", npc[s] }
    }

    ###########################################################################
    on_deep_copy lambda { |original|
      gain_level while level < original.level
    }

    ###########################################################################
    def _interpret_name
      '$being'
    end

    ###########################################################################
    def summary
      $templates.npc_summary % [
        rid, name,
        id,
        level,
        anatomy.name,
        strength.current, health.current, health.max,
        dexterity.current, mana.current, mana.max,
        constitution.current, stamina.current,
        intelligence.current, hit_bonus.current,
        focus.current, damage_bonus.current,
        creativity.current, armor.current,
        charisma.current, bash.current,
        luck.current, slash.current,
        pierce.current,
        exotic.current
      ]
    end

    ###########################################################################
    vivify lambda {
      $world.schedule Mechanics::RecurringJob.new (5..30), self, (5..30), :auto_act
    }

    ###########################################################################
    def auto_act
      case rand(1..2)
      when 1 then auto_move
      when 2 then auto_attack
      end
    end

    ###########################################################################
    def auto_move
      return if mobility.zero?

      speak 'moving!'
    end

    ###########################################################################
    def auto_attack
      return if hostility.zero?

      speak 'attacking!'
    end
  end
end
