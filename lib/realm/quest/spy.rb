# Spy is meant as an abstract base class for Objective and Parameter, which
# share a lot of conceptual functionality since both are ultimately just
# monitoring hooks on the hero Objekt.

module Realm
  class Spy < Objekt
    attr_accessor :task, :name, :trigger, :params, :active, :status

    def initialize(id = nil, props = {})
      @task    = nil
      @name    = nil
      @trigger = nil
      @params  = nil
      @active  = false
      @status  = nil # :completed or :failed
      @state   = {} # instance-specific data
      super
    end

    on_hashify lambda {
      hash = {
        name: @name,
        trigger: @trigger,
        params: @params,
        active: @active,
        status: @status,
        state: @state
      }
      { spy: hash }
    }

    on_dehashify lambda { |hash|
      hash = hash[:spy]

      @name = hash[:name]
      @trigger = hash[:trigger]
      @params = hash[:params]
      @active = hash[:active]
      @status = hash[:status]
      @state = hash[:state]
    }

    # Activating the Spy will cause it to monitor the underlying Objekt (the
    # 'hero') for relevant events.
    def activate(task)
      Symphony.logger.trace 'Spy %s (%s) #activate for %s' % [fqid_str, task.fqid_str, name]
      @task = task
      @active = true
      task.hero.register_effect trigger, self if trigger

      activate_meth = "activate_#{name}".to_sym
      send activate_meth if respond_to? activate_meth
    end

    def deactivate
      Symphony.logger.trace 'Spy %s (%s) #deactivate for %s' % [fqid_str, task.fqid_str, name]
      @active = false
      task.hero.unregister_effect trigger, self if trigger

      deactivate_meth = "deactivate_#{name}".to_sym
      send deactivate_meth if respond_to? deactivate_meth
    end

    def complete!
      Symphony.logger.trace 'Spy %s (%s) #complete! for %s' % [fqid_str, task.fqid_str, name]
      @status = :complete
      task.run_hook :spy_completed
    end

    def fail!
      Symphony.logger.trace 'Spy %s (%s) #fail! for %s' % [fqid_str, task.fqid_str, name]
      @status = :failed
      task.run_hook :spy_failed
    end

    def update!
      Symphony.logger.trace 'Spy %s (%s) #update! for %s' % [fqid_str, task.fqid_str, name]
      task.run_hook :spy_updated
    end

    # A Spy can be configured and 'ready to go' but not activated. Take the
    # example of a player who has a quest available, but hasn't accepted it.
    def active?
      @active == true
    end

    def complete?
      @status == :complete
    end

    def failed?
      @status == :failed
    end

    # Subclasses define the #valid_symbols method as appropriate for their use
    # and it returns an array of symbols.
    def validate_name(name)
      return true if valid_symbols.include? name

      Symphony.logger.error 'Spy %s from %s - bad name (%s) from %s' % [
        fqid_str,
        task.fqid_str,
        name.to_s,
        caller_locations(1, 1)[0].label
      ]
      false
    end

    # The #setup method must be called before #activate and will invoke the
    # appropriate matching subclass setup_* method.
    def setup(name, args = nil)
      return false unless validate_name name

      case self
      when Objective
        @trigger = name
      when Parameter
        trig = Konst::SPY_PARAMETERS[name]
        if !trig.nil? && !Konst::SPY_TRIGGERS.include?(trig)
          Symphony.logger.error 'Spy %s from %s - bad trigger (%s) for parameter %s' % [
            fqid_str,
            task.fqid_str,
            trig,
            name
          ]
          return
        end
        @trigger = trig
      else
        raise 'You cannot #setup Realm::Spy directly - you need to use a subclass'
      end

      @name = name
      setup_meth = "setup_#{name}".to_sym
      send setup_meth, *args if respond_to? setup_meth
    end

    # This is the public entry method for invoking whatever subclass- and type-
    # specific logic needs to be run to evaluate Spy state.
    def exec(arg = nil)
      return false unless validate_name name

      callback_method = "exec_#{name}".to_sym
      callback_retval = send callback_method, arg

      case callback_retval
      when :complete then complete!
      when :fail then fail!
      when :update then update!
      end
    end
  end
end
