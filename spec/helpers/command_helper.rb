module Command
  class CommandApple < Command
    def initialize
      @name = 'apple'
    end

    def execute(client, arguments); end
  end

  class CommandBanana < Command
    def initialize
      @name = 'banana'
    end

    def execute(client, arguments); end
  end

  class CommandCherry < Command
    def initialize
      @name = 'cherry'
    end

    def execute(client, arguments); end
  end

  class CommandWithExecute < Command
    def initialize
      @name = 'foo'
    end

    def execute(client, arguments); end
  end

  class CommandWithoutExecute < Command
    def initialize
      @name = 'bar'
    end
  end
end
