module Command

  class Command_apple < Command
    def initialize
      @name = "apple"
    end
    def execute(client, arguments)
    end
  end

  class Command_banana < Command
    def initialize
      @name = "banana"
    end
    def execute(client, arguments)
    end
  end

  class Command_cherry < Command
    def initialize
      @name = "cherry"
    end
    def execute(client, arguments)
    end
  end

  class Command_with_execute < Command
    def initialize
      @name = "foo"
    end
    def execute(client, arguments)
    end
  end

  class Command_without_execute < Command
    def initialize
      @name = "bar"
    end
  end
end
