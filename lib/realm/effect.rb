# TODO: Effects that wear out with usage, not time (like poisoning a weapon)
# TODO: Save & restore permanent effects
module Realm
  class Effect < Objekt
    attr_reader :name, :originator, :target
    attr_reader :duration, :recur_delay
    attr_reader :expiry_job, :execution_jobs
    attr_reader :announce_apply, :announce_expire
    attr_reader :modifiers
    attr_reader :valid_for
    attr_reader :can_stack

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
      @valid_for       = []
      @can_stack       = false
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
      return nil unless validate

      link_effect
      create_jobs
      modify if modifiers.any?
      _apply if respond_to? :_apply
      announce if announce_apply
      self
    end

    ###########################################################################
    def validate
      raise 'Effect %s has no target' % [self.class.name] if target.nil?

      if valid_for.none? { |vf| target.is_a? vf }
        raise 'Invalid target %s for %s (only accepts %s)' % [
          target.fqid_str,
          self.class.name,
          valid_for.map(&:to_s).join(', ')
        ]
      end

      return false if !can_stack && target.effects.any? { |e| e.name == name }

      true
    end

    ###########################################################################
    # Link to the target Objekt.
    def link_effect
      target.effects.add self
    end

    ###########################################################################
    # Create jobs to manage the effect.
    def create_jobs
      # Should we create an expiry job?
      if duration.positive?
        @expiry_job = Mechanics::Job.new duration, self, :expire
        $world.schedule expiry_job
      end

      # Should we create a routine worker job?
      if respond_to? :_recur
        j = Mechanics::RecurringJob.new 0, self, recur_delay, :recur
        $world.schedule j
        execution_jobs.append j
      end

      true
    end

    ###########################################################################
    # Apply any modifiers, if defined.
    def modify
      case target
      when Realm::Being
        target.modify modifiers
      when Realm::Item
        target.modify modifiers
        # If we are adding modifiers to an Item we then have to check if
        # the Item is equipped to a Being, and if so, ensure that those
        # modifiers are applied to the Being.
        target.slotted_to.being.modify modifiers if target.slotted_to.is_a? Talent::Anatomy
      else
        Symphony.logger.error '%s cannot modify %s' % [self.class.name, target.class.name]
      end
      target.modify modifiers unless modifiers.empty?
    end

    ###########################################################################
    def announce
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
      expiry_job&.kill

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

    ###########################################################################
    def to_s
      out = ''
      out << name
      if modifiers.any?
        out << ' ('
        out << modifiers.map do |k, v|
          '%s %s%d{x' % [k.to_s, v.positive? ? '{G+' : '{R', v]
        end.join(', ')
        out << ')'
      end

      out
    end
  end
end
