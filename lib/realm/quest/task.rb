# Task is the "main" workhorse class of the questing system. It encapsulates
# the objective, requirements, and rewards of a particular quest segment and
# keeps the hero Objekt coordinated with those details.
#
module Realm
  class Task < Objekt
    attr_accessor :description, :objective, :parameters,
                  :reward, :parents, :children, :quest

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
      @quest       = nil
      @objective   = nil
      @parameters  = []
      @reward      = nil
      @parents     = []
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

    def hero
      @quest.hero
    end

    # Note that Task only partially implements serialization hooks and relies on
    # logic in Quest serialization hooks to manipulate parent/child relationships
    on_hashify lambda {
      hash = {
        description: description,
        objective: objective&.hashify,
        parameters: parameters&.map(&:hashify),
        reward: reward&.hashify
      }
      { task: hash }
    }

    on_dehashify lambda { |hash|
      hash = hash[:task]
      @description = hash[:description]
      @objective = Realm::Objective.dehashify(hash[:objective]) if hash[:objective]
      hash[:parameters].each { |p| @parameters << Realm::Parameter.dehashify(p) }
      @reward = Realm::Reward.dehashify(hash[:reward]) if hash[:reward]
    }

    on_deep_copy lambda { |original|
      @description = original.description
      @objective = original.objective.clone
      original.parameters.each { |p| @parameters << p.clone }
      @reward = original.reward.clone
    }

    enervate lambda {
      @objective.run_hook :enervate
      @parameters.each { |p| p.run_hook :enervate }
      @reward.run_hook :enervate
      dispose
    }

    spy_completed lambda {
      Symphony.logger.debug '%s :spy_completed for %s' % [fqid_str, @quest.hero.fqid_str]
      evaluate_spies
    }

    spy_failed lambda {
      Symphony.logger.debug '%s :spy_failed for %s' % [fqid_str, @quest.hero.fqid_str]
      evaluate_spies
    }

    spy_updated lambda {
      Symphony.logger.trace '%s :spy_updated for %s' % [fqid_str, @quest.hero.fqid_str]
      evaluate_spies
    }

    # There are a few places where we need to remove a task from its heirarchy,
    # and in those cases parent/child relationships have to be adjusted.
    def pluck
      if parents.any?
        # If self has any parents, we need to make said parents aware of each of
        # self's children.
        parents.each do |p|
          p.children.delete self
          p.children |= children
        end
        # If self has any children, we need to make said children aware of each
        # of self's parents.
        children.each do |c|
          c.parents.delete self
          c.parents |= parents
        end
      else
        # This is a root Task, and so it's current children will become root
        # Tasks themselves.
        children.each { |c| c.parents.delete self }
      end
    end

    def add_parent(new_parent)
      # if this is a root task, remove if from the quest
      @quest.tasks.delete self if parents.empty?
      @parents << new_parent
      new_parent.children << self
    end

    def remove_parent(old_parent)
      # if this is the last parent, add the task back into the quest as a root
      @quest.tasks << self if @parents == [old_parent]
      @parents.delete old_parent
      old_parent.children.delete self
    end

    def add_child(new_child)
      @children << new_child
      new_child.parents << self
    end

    def remove_child(old_child)
      @children.delete old_child
      old_child.parents.delete self
    end

    # Each time a Spy is updated, we need to recheck the state of all of them
    # to figure out if the Task is complete.
    def evaluate_spies
      if complete?
        @quest.run_hook :task_completed, self
        finish
      elsif failed?
        @quest.run_hook :task_failed, self
        finish
      else
        @quest.run_hook :task_updated, self
      end
    end

    # Finishing the Task will cause deactivation.
    def finish
      Symphony.logger.trace '%s #finish for %s' % [fqid_str, @quest.hero.fqid_str]
      deactivate
    end

    # Spies don't automatically listen for events - they need to be activated
    def activate
      objective.activate self
      parameters.each { |p| p.activate self }
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

    def change_reward_value(kind, value)
      @reward = Realm::Reward.new if @reward.nil? && value.positive?
      @reward.send "#{kind}=", value if %w[silver gold exp].include? kind
      @reward = nil if @reward.empty?
    end

    def add_reward_item(item_id)
      @reward ||= Realm::Reward.new
      @reward.item_ids << item_id
    end

    def remove_reward_item(item_id)
      @reward.item_ids.delete item_id
      @reward = nil if @reward.empty?
    end
  end
end
