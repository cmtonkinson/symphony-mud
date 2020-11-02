# TODO: Apply effects to other Objekt types (like adding modifiers to armor)
# TODO: Effects that wear out with usage, not time (like poisoning a weapon)
# TODO: Save & restore permanent effects
# TODO: Prevent duplicate effects (but should we allow stacking?)
module Realm
  class Effect < Objekt
    attr_reader :name, :originator, :target
    attr_reader :duration, :recur_delay
    attr_reader :expiry_job, :execution_jobs
    attr_reader :announce_apply, :announce_expire
    attr_reader :modifiers

    ###########################################################################
    def initialize(id = nil, props = {})
      @name            = ''
      @originator      = nil
      @target          = nil
      @duration        = 0
      @recur_delay     = 0
      @expiry_job      = nil
      @execution_jobs  = []
      @announce_apply  = false
      @announce_expire = false
      @modifiers       = {}
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
      @expiry_job = Mechanics::Job.new duration, self, :expire
      $world.schedule expiry_job

      # Create an execution job, if defined.
      if respond_to? :_recur
        j = Mechanics::RecurringJob.new 0, self, recur_delay, :recur
        $world.schedule j
        execution_jobs.append j
      end

      # Apply any modifiers, if defined.
      target.modify modifiers unless modifiers.empty?

      # Run custom code, if defined.
      _apply if respond_to? :_apply

      # Announce, if defined.
      return unless announce_apply

      target.puts I18n.t("effect.#{name}.apply_target")
      target.room.puts I18n.t("effect.#{name}.apply_room"), target, exclude: target
    end

    ###########################################################################
    def expire
      # Run custom code, if defined.
      _expire if respond_to? :_expire

      # Remove all defined modifiers.
      target.modify modifiers, true unless modifiers.empty?

      # Kill execution job, if defined.
      execution_jobs.each(&:kill)

      # Kill expiry job.
      expiry_job.kill

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
    def recur
      _recur
    end

    ###########################################################################
    def expires_in
      @expiry_job.wen - Time.now.to_i
    end
  end
end
