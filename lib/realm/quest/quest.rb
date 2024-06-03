# Quest is the top-level object in the questing system. It encapsulates
# a list of Tasks along with associated metadata and is designed with an
# interface to be manipulated directly by users and builders alike.
module Realm
  class Quest < Objekt
    attr_accessor :tasks, :hero, :activation, :status, :hidden

    def initialize(id = nil, props = {})
      @tasks      = [] # Tasks to be completed
      @hero       = nil # Objekt Burdened with the Glorious Purpose(tm) of this Quest
      @activation = nil # Conditions under which this Quest is made available to the hero
      @status     = :template # As seen by the player
      @hidden     = false
      super
    end

    # TODO: implement status based on tasks

    on_hashify lambda {
      h = {}
      h[:status] = @status
      h[:hidden] = @hidden
      h[:activation] = @activation.hashify if @activation
      h[:tasks] = []
      h[:task_children] = {}

      # All Tasks are stored flattened, and then their relationships are
      # mapped separately. It makes the tree easier to reconstruct during
      # dehashify.
      all_tasks_flattened.each do |t|
        h[:tasks] << t.hashify
        h[:task_children][t.id] = t.children.map(&:id) if t.children
      end
      { quest: h }
    }

    # See doc/quest.md for detail about how and why Tasks are [de]serialized
    # in the Quest [de]serialization hooks.
    on_dehashify lambda { |hash|
      h = hash[:quest]
      @status = h[:status]
      @hidden = h[:hidden]
      if h[:activation]
        @activation = Realm::Activation.dehashify(h[:activation])
        @activation.quest = self
      end
      @tasks = []
      if h[:tasks]
        h[:tasks].each do |t|
          task = Realm::Task.dehashify(t)
          task.quest = self
          @tasks << task
        end
      end
      @tasks.each do |t|
        next unless h[:task_children][t.id]

        h[:task_children][t.id].each do |child_id|
          child = @tasks.find { |c| c.id == child_id }
          unless child
            Symphony.logger.error 'Task child_id %s not found during dehashification of %s' % [child_id, t.fqid_str]
            next
          end
          child.add_parent t
        end
      end
    }

    on_deep_copy lambda { |original|
      @status = original.status
      @hidden = original.hidden
      @activation = original.activation.clone
      @activation.quest = self
      @tasks = []
      original.tasks.each do |t|
        task = t.clone
        task.quest = self
        @tasks << task
      end
    }

    vivify lambda {
      @activation.run_hook :vivify if @activation
    }

    enervate lambda {
      @hero&.quests&.delete self if @hero
      @tasks.each { |t| t.run_hook :enervate }
      @activation.run_hook :enervate
    }

    task_completed lambda { |task|
      @hero.run_hook :task_completed, task
      task.quest.evaluate_state
    }

    task_failed lambda { |task|
      @hero.run_hook :task_failed, task
      task.quest.evaluate_state
    }

    task_updated lambda { |task|
      @hero.run_hook :task_updated, task
    }

    # This returns all Tasks in the Quest Task heirarchy in a flattened Array.
    # This is an iterative solution to the recursive problem because it needs
    # to live within the Quest class (yea yea I hear you... coupling... ew).
    def all_tasks_flattened
      final_task_list = []
      new_children = []

      new_children |= @tasks
      while new_children.any?
        next_task = new_children.shift
        final_task_list << next_task
        new_children |= next_task.children
      end

      final_task_list
    end

    # TODO: refactor:
    #   1. delegate output to hero via callback hook (DONE)
    #   2. does this account for task heirarchy? (eg children failure/success)
    def evaluate_state
      if @tasks.all?(&:complete?)
        @status = :complete
        @hero.run_hook :quest_complete, self
      elsif @tasks.any?(&:failed?)
        @status = :failed
        @hero.run_hook :quest_failed, self
      end
    end

    def task_by_rid(rid)
      all_tasks_flattened.find { |t| t.rid == rid }
    end

    def next_task_rid
      @tasks.empty? ? 1 : (@tasks.map(&:rid).max + 1)
    end

    def activate
      @tasks.each(&:activate)
    end

    def assigned?
      @status == :available
    end

    def assign!
      @status = :available
    end

    def accepted?
      @status == :accepted
    end

    def accept!
      @status = :accepted
      activate
    end

    def complete?
      @status == :complete
    end

    def complete!
      @status = :complete
    end

    def failed?
      @status == :failed
    end

    def failed!
      @status = :failed
    end

    def hidden?
      @hidden
    end

    def hide!
      @hidden = true
    end

    def unhide!
      @hidden = false
    end

    def summary
      str = "Quest #{name} / #{rid} / #{id}"
      str << "\nActivation: #{@activation.trigger} (#{@activation.params.to_json})" if @activation
      tasks.each { |t| str << task_summary(t) }
      str
    end

    def task_summary(task, indent = 0)
      i = "\n#{'  ' * indent}"
      str = "#{i}Task #{task.rid}: #{task.name}"
      str << "#{i}  - objective: #{task.objective.name} (#{task.objective.params.to_json})" if task.objective
      task.parameters.each { |p| str << "#{i}  - parameter: #{p.name} (#{p.params.to_json})" }
      task&.reward&.to_str_array&.each { |s| str << "#{i}  - reward: #{s}" }
      task.children.each { |c| str << task_summary(c, indent + 1) }
      str
    end
  end
end
