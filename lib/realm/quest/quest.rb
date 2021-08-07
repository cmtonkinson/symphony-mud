# Quest is the top-level object in the questing system. It encapsulates
# a list of Tasks along with associated metadata and is designed with an
# interface to be manipulated directly by users and builders alike.

module Realm
  class Quest < Objekt
    attr_accessor :tasks

    def initialize(id = nil, props = {})
      @tasks = []
      super
    end

    on_hashify lambda {
      hash = {
        tasks: tasks.map(&:hashify)
      }
      { quest: hash }
    }

    on_dehashify lambda { |hash|
      hash = hash[:quest]
      @tasks = []
      hash[:tasks].each { |h| @tasks << Realm::Task.dehashify(h) }
    }

    on_deep_copy lambda { |original|
      @tasks = []
      original.tasks.each { |t| @tasks << t.clone }
    }

    enervate lambda {
      @tasks.each { |t| t.run_hook :enervate }
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
      tasks.each { |t| str << task_summary(t) }
      str
    end

    def task_summary(task, indent = 0)
      i = "\n" + ('  ' * indent)
      str = "#{i}Task #{task.rid}: #{task.name}"
      str << "#{i}  objective: #{task.objective.name} / #{task.objective.params}" if task.objective
      task.parameters.each { |p| str << "#{i}  parameter: #{p.name} / #{p.params}" }
      task&.reward&.to_str_array&.each { |s| str << "#{i}  reward: #{s}" }
      task.children.each { |c| str << c.task_summary(c, indent + 1) }
      str
    end
  end
end
