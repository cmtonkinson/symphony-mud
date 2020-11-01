module Realm
  class Effect < Objekt
    attr_reader :name, :originator, :target
    attr_reader :duration, :expiry_job
    attr_reader :announce_apply, :announce_expire

    ###########################################################################
    def initialize(id = nil, props = {})
      @name            = ''
      @originator      = nil
      @target          = nil
      @duration        = 0
      @announce_apply  = false
      @announce_expire = false
      super
    end

    ###########################################################################
    on_hashify lambda {
    }

    ###########################################################################
    on_dehashify lambda { |hash|
    }

    ###########################################################################
    def apply
      raise 'Effect %s has no target' % [self.class.name] if target.nil?

      # Link to the target Objekt.
      target.effects.add self

      # Create an expiry job.
      @expiry_job = Mechanics::Job.new(duration, self, :expire)
      $world.schedule expiry_job

      # Create an execution job, if defined.

      # Apply any modifiers, if defined.

      # Run custom code, if defined.
      _apply if respond_to? :_apply

      # Announce, if defined.
      if announce_apply
        target.puts I18n.t("effect.#{name}.apply_target")
        target.room.puts I18n.t("effect.#{name}.apply_room"), target, exclude: target
      end
    end

    ###########################################################################
    def expire
      # Run custom code, if defined.
      _expire if respond_to? :_expire

      # Remove all defined modifiers.

      # Kill execution job, if defined.

      # Kill expiry job.

      # Announce, if defined.
      if announce_expire
        target.puts I18n.t("effect.#{name}.expire_target")
        target.room.puts I18n.t("effect.#{name}.expire_room"), target, exclude: target
      end

      # Get rid of it.
      target.effects.delete self
      dispose
    end

    ###########################################################################
    def expires_in
      @expiry_job.wen - Time.now.to_i
    end
  end
end
