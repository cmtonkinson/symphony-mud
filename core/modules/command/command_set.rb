module Command

  class CommandSet < ::SortedSet
    attr_accessor :name

    def initialize(name)
      @name = name
      super nil
    end

    def self.factory(name, commands)
      set = CommandSet.new name
      commands.each { |c| set << c }
      set
    end

  end

end
