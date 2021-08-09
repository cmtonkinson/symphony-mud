# Quest is the top-level object in the questing system. It encapsulates
# a list of Tasks along with associated metadata and is designed with an
# interface to be manipulated directly by users and builders alike.
module Realm
  class Quest < Objekt
    attr_accessor :tasks, :hero, :activation

    def initialize(id = nil, props = {})
      @tasks = []       # Tasks to be completed
      @hero  = nil      # Objekt burdened with the Glorious Purpose(tm) of this Quest
      @activation = nil # Conditions under which this Quest is made available to the hero
      super
    end

    on_hashify lambda {
      h = {}

      h[:tasks] = @tasks.map(&:hashify)
      h[:activation] = @activation.hashify if @activation

      { quest: h }
    }

    on_dehashify lambda { |hash|
      h = hash[:quest]

      @tasks = []
      h[:tasks].each { |t| @tasks << Realm::Task.dehashify(t) } if h[:tasks]

      if h[:activation]
        @activation = Realm::Activation.dehashify(h[:activation])
        @activation.quest = self
      end
    }

    on_deep_copy lambda { |original|
      @tasks = []
      original.tasks.each { |t| @tasks << t.clone }

      @activation = original.activation.clone
      @activation.quest = self
    }

    vivify lambda {
      @activation.run_hook :vivify if @activation
    }

    enervate lambda {
      @tasks.each { |t| t.run_hook :enervate }
      @activation.run_hook :enervate
    }

    def task_by_rid(rid)
      @tasks.flat_map(&:flat_tree).find { |t| t.rid == rid }
    end

    def next_task_rid
      return 1 if @tasks.empty?

      @tasks.flat_map(&:flat_tree).max(&:rid).rid + 1
    end

    def summary
      str = "Quest #{name} / #{rid} / #{id}"
      str << "\nActivation: #{@activation.trigger} (#{@activation.params.to_json})" if @activation
      tasks.each { |t| str << task_summary(t) }
      str
    end

    def task_summary(task, indent = 0)
      i = "\n" + ('  ' * indent)
      str = "#{i}Task #{task.rid}: #{task.name}"
      str << "#{i}  - objective: #{task.objective.name} (#{task.objective.params.to_json})" if task.objective
      task.parameters.each { |p| str << "#{i}  - parameter: #{p.name} (#{p.params.to_json})" }
      task&.reward&.to_str_array&.each { |s| str << "#{i}  - reward: #{s}" }
      task.children.each { |c| str << c.task_summary(c, indent + 1) }
      str
    end
  end
end
