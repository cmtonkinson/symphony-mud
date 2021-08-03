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
      hash[:tasks].each { |t| @tasks << Realm::Task.dehashify(t) }
    }

    on_deep_copy lambda { |original|
      @tasks = []
      original.tasks.each { |t| @tasks << t.clone }
    }

    enervate lambda {
      @tasks.each { |t| t.run_hook :enervate }
    }
  end
end
