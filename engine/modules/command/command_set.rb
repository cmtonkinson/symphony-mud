module Command

  class CommandSet < ::SortedSet

    def initialize
      super
    end

    def self.factory(commands)
      command_set = CommandSet.new
      commands.each { |command| command_set << command }
      command_set
    end

  end

end
