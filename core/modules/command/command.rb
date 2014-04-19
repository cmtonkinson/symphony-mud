module Command

  class Command
    attr_accessor :name

    def initialize
    end

    def <=> (other)
      self.name <=> other.name
    end

    def execute(client, arguments)
      raise NoMethodError.new "Command::Command#execute is undefined for Command '#{@name}'"
    end

  end

end
