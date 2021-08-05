# Task is the "main" workhorse class of the questing system. It encapsulates
# the objective, requirements, and rewards of a particular quest segment and
# keeps the tasked Objekt coordinated with those details.

module Realm
  class Task < Objekt
    attr_accessor :description # player-friendly
    attr_accessor :tasked # what Objekt is burdened to complete this?
    attr_accessor :objective # what's the thing that has to be done?
    attr_accessor :parameters # are there restrictions on achieving the objective?
    attr_accessor :rewards
    attr_accessor :parent, :children

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

    def initialize(id = nil, props = {})
      @description = ''
      @tasked      = nil
      @objective   = nil
      @parameters  = []
      @rewards     = []
      @parent      = nil
      @children    = []
      super
    end

    # We use title as the public API, but it's the Objekt built-in 'name'
    # property that is actually used under the hood.
    def title=(title)
      @name = title
    end

    def title
      @name
    end

    on_hashify lambda {
      hash = {
        description: description,
        objective: objective&.hashify,
        parameters: parameters&.map(&:hashify),
        rewards: rewards.map(&:hashify),
        children: children.map(&:hashify)
      }
      { task: hash }
    }

    on_dehashify lambda { |hash|
      hash = hash[:task]

      @description = hash[:description]
      @objective = Realm::Objective.dehashify(hash[:objective]) if hash[:objective]

      hash[:parameters].each { |param| @parameters << Realm::Parameter.dehashify(param) }

      hash[:rewards].each { |reward| @rewards << Realm::Reward.dehashify(reward) }

      hash[:children].each do |c|
        c = Realm::Task.dehashify(c)
        c.parent = self
        @children << c
      end
    }

    on_deep_copy lambda { |original|
      @description = original.description
      @objective = original.objective.clone

      original.parameters.each { |param| @parameters << param.clone }

      original.rewards.each { |reward| @rewards << reward.clone }

      original.children.each do |c|
        child = c.clone
        child.parent = self
        @children << child
      end
    }

    enervate lambda {
      @objective.run_hook :enervate
      @parameters.each { |p| p.run_hook :enervate }
      @rewards.each { |r| r.run_hook :enervate }
      @children.each { |c| c.run_hook :enervate }
      dispose
    }

    spy_completed lambda {
      Symphony.logger.debug '%s :spy_completed for %s' % [fqid_str, tasked.fqid_str]
      evaluate_spies
    }

    spy_failed lambda {
      Symphony.logger.debug '%s :spy_failed for %s' % [fqid_str, tasked.fqid_str]
      evaluate_spies
    }

    spy_updated lambda {
      Symphony.logger.trace '%s :spy_updated for %s' % [fqid_str, tasked.fqid_str]
      evaluate_spies
    }

    # Each time a Spy is updated, we need to recheck the state of all of them
    # to figure out if the Task is complete.
    def evaluate_spies
      if complete?
        tasked.run_hook :task_completed, self
        finish
      elsif failed?
        tasked.run_hook :task_failed, self
        finish
      else
        tasked.run_hook :task_updated, self
      end
    end

    # Finishing the Task will cause deactivation.
    def finish
      Symphony.logger.trace '%s #finish for %s' % [fqid_str, tasked.fqid_str]
      deactivate
    end

    # Spies don't automatically listen for events - they need to be activated
    def activate
      objective.activate
      parameters.each(&:activate)
    end

    def deactivate
      objective.deactivate
      parameters.each(&:deactivate)
    end

    # Tests whether the Objective has been met and none of the auxillary
    # requirements (Parameters) have been violated.
    def complete?
      objective.complete? && parameters.none?(&:failed?)
    end

    def failed?
      parameters.any?(&:failed?)
    end

    def set_objective(name, args = nil)
      @objective = Objective.new
      @objective.task = self
      @objective.setup(name, args)
    end

    def add_parameter(name, args = nil)
      p = Parameter.new
      p.task = self
      p.setup(name, args)
      @parameters << p
    end

    # Recursive method to return a flat array of this Task, and all descendent
    # Tasks.
    def flat_tree
      array = [self]
      children.each { |c| array += c.flat_tree }
      array
    end
  end
end
