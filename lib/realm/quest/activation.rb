# Activations are used to make Quests available to a Being. Similar in concept
# to a Spy, but the Activation watches for events from the perspective of the
# world/environment. For example, a Room watches for beings entering, or an
# item watches for being picked up, etc. At the time the Activation fires,
# the Quest is cloned and activated on the Objekt.
#
# Conceptually this is similar to the Spy (Objective and Paramater) mechanics
# but the implementation is greatly simplified since it's based solely on
# effect_triggers using a standardized syntax.
module Realm
  class Activation < Objekt
    attr_accessor :quest, :trigger, :params

    def initialize(id = nil, props = {})
      @quest   = nil # The Quest
      @trigger = nil # Name of the effect_trigger
      @params  = nil # Arbitrary (serializable) data for internal use per trigger type
      @target  = nil # The Objekt which is monitored for the effect_trigger
      super
    end

    on_hashify lambda {
      hash = {
        trigger: @trigger,
        params: @params
      }
      { activation: hash }
    }

    on_dehashify lambda { |hash|
      h = hash[:activation]
      @trigger = h[:trigger]
      @params  = h[:params]
    }

    vivify lambda {
      case @trigger
      when :enter_room
        r = $world.find_room @params.to_i
        if r.rid == 1
          Symphony.logger.error 'Activation of %s failed (searched rid %s)' % [quest.fqid_str, @params] if r.rid == 1
        else
          setup :enter_room, r
          Symphony.logger.trace 'Activated %s on %s for %s' % [quest.fqid_str, r.fqid_str, 'enter_room']
        end
      else
        Symphony.logger.error 'Cannot vivify %s for %s (trigger %s/%s)' % [fqid_stR, quest.fqid_str, @trigger, @params]
      end
    }

    enervate lambda {
    }

    # Used to link the `trigger` effect on the `@target`.
    def setup(trigger, target)
      unless Konst::ACTIVATION_TRIGGERS.include? trigger
        Symphony.logger.error '%s is not a valid activation trigger for %s (%s)' % [trigger, target.fqid_str, quest]
        return false
      end

      @target = target
      @target.register_effect trigger, self
    end

    # Dissociates the `trigger` effect from the `@target`.
    def teardown
      @target.unregister_effect trigger, self
      @target = nil
    end

    # Invoked by the Objekt/effects code when the `trigger` is invoked. It is
    # passed the Objekt that caused/invoked the trigger. If all requirements
    # are met, the Objekt will be given the Quest and exec() returns true.
    def exec(subject)
      # Don't allow multiple instances of the same Quest.
      return false if subject.quests.any? { |q| q.cloned_from == @quest.id }

      # Should we activate the Quest?
      test_meth = "test_#{trigger}".to_sym
      test_result = send test_meth, subject
      return false unless test_result

      # Activate the Quest.
      Symphony.logger.trace '%s activated via %s by %s' % [fqid_str, trigger, subject.fqid_str]
      # TODO: activate a scene, if configured
      subject.assign_quest @quest
      true
    end

    # Invoked on a Room when a Being enters it, returns true when further
    # action should be taken, false otherwise.
    def test_enter_room(being)
      # Don't bother giving Quests to NPCs.
      return false unless being.is_a? Realm::Avatar

      true
    end
  end
end
