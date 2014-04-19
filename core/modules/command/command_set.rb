module Command

  class CommandSet < SortedSet
    attr_reader :name

    def initialize(name)
      @name = name
    end

  end

end
