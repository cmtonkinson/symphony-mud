module Realm
  class Task < Objekt
    attr_accessor :name, :description # player-friendly
    attr_accessor :tasked # what Objekt is burneded to complete this?
    attr_accessor :objective # what's the thing that has to be done?
    attr_accessor :parameters # are there restrictions on achieving the objective?

    define_hooks :spy_completed, :spy_failed, :spy_updated

    # * name
    # * description

    # parameters...
    #  time limits
    #  modifiers
    #  effects
    #  prohibitions (e.g. can't sleep, speak, fight)
    # TODO: Implement effect_triggers for all of these
    # TODO: Create Job-able components

    # results...
    #  gold
    #  items
    #  modifiers
    #  effects
    #  exp
    #  npcs (enemies, pets)
    #  schedule jobs, job sequences    def initialize
    #  begin a scene

    # status...
    #  must be serializable (to save/load)

    ###########################################################################
    def initialize(id = nil, props = {})
      @name        = ''
      @description = ''
      @tasked      = nil
      @objective   = nil
      @parameters  = []
      super
    end

    ###########################################################################
    after_initialize lambda {
    }

    ###########################################################################
    on_hashify lambda {
      hash = {
        name: name,
        description: description,
        objective: objective.hashify,
        parameters: parameters.map(&:hashify)
      }
      { task: hash }
    }

    ###########################################################################
    on_dehashify lambda { |hash|
      hash = hash[:task]

      @name = hash[:name]
      @description = hash[:description]
      @objective = Realm::Objective.dehashify(hash[:objective])

      @parameters = []
      hash[:parameters].each { |param| @parameters << Realm::Parameter.dehashify(param) }
    }

    ###########################################################################
    on_deep_copy lambda { |original|
      @name = original.name
      @description = original.description
      @objective = original.objective.clone

      @parameters = []
      @parameters.each { |param| @parameters << param.clone }
    }

    ###########################################################################
    enervate lambda {
      @objective.run_hook :enervate
      @parameters.each { |p| p.run_hook :enervate }
      dispose
    }

    ###########################################################################
    spy_completed lambda {
      Symphony.logger.debug '%s :spy_completed for %s' % [fqid_str, tasked.fqid_str]
      finish
    }

    ###########################################################################
    spy_failed lambda {
      Symphony.logger.debug '%s :spy_failed for %s' % [fqid_str, tasked.fqid_str]
      finish
    }

    ###########################################################################
    spy_updated lambda {
      Symphony.logger.debug '%s :spy_updated for %s' % [fqid_str, tasked.fqid_str]
    }

    ###########################################################################
    def finish
      Symphony.logger.trace '%s #finish for %s' % [fqid_str, tasked.fqid_str]
      objective.deactivate
      parameters.each(&:deactivate)
    end

    ###########################################################################
    def complete?
      objective.complete? && parameters.none(&:failed?)
    end

    ###########################################################################
    def set_objective(name, args = nil)
      @objective = Objective.new.setup(name, args)
    end

    ###########################################################################
    def remove_objective
      @objective = nil
    end

    ###########################################################################
    def add_parameter(name, args = nil)
      @parameters << Parameter.new.setup(name, args)
    end
  end
end
