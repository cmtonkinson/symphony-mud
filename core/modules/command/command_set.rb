module Command

  class CommandSet < ::SortedSet

    def initialize
      super
    end

    def self.factory(commands)
      set = CommandSet.new
      commands.each { |c| set << c }
      set
    end

  end

end
