module Realm
  class Objekt
    attr_reader :effects, :effect_triggers

    ###########################################################################
    after_initialize lambda {
      @effects = Set.new
      @effect_triggers = {}
    }

    ###########################################################################
    enervate lambda {
      effects.first.expire until effects.empty?
      effect_triggers.clear
    }

    ###########################################################################
    on_hashify lambda {
      hash = {}
      hash[:effects] = effects.map(&:hashify)
      hash
    }

    ###########################################################################
    on_dehashify lambda { |hash|
      return unless hash.key? :effects

      hash[:effects].each do |h|
        e = Realm::Effect.dehashify(h)
        e.target = self
        effects.add e
      end
    }

    ###########################################################################
    def register_effect(trigger_name, effect)
      effect_triggers[trigger_name] ||= []
      effect_triggers[trigger_name] << effect
    end

    ###########################################################################
    def unregister_effect(trigger_name, effect)
      effect_triggers[trigger_name].delete effect
      effect_triggers.delete trigger_name if effect_triggers[trigger_name].empty?
    end

    ###########################################################################
    def trigger_effect(trigger_name, invoker)
      return unless effect_triggers.key? trigger_name

      effect_triggers[trigger_name].each { |e| e.use invoker }
    end
  end
end
