module Command

  class Command
    attr_accessor :name, :level, :exec

    def initialize
      @name     = "Command"
      @level    = 1
      @args     = []
      @num_args = 0
      @exec     = lambda { raise NoMethodError.new "Command::Command.exec is undefined for Command '#{@name}'" }
    end

    def <=> (other)
      self.name <=> other.name
    end

    def perform(user, full_command)
      # Parse the command, according to @args.
      arg_string = full_command.split(" ")[1..-1].join " "
      # These variables will be available to the exec block as instance variables.
      state = {
        "@user"         => user,
        "@full_command" => full_command,
        "@arg_string"   => arg_string,
      }
      # Set the instance variables from above.
      state.each { |name,value| self.instance_variable_set name, value }
      # Pass control to the Command exec Proc.
      @exec.call
      # Clean up the instance variables created above.
      state.each { |name,value| self.remove_instance_variable name }
    end

    def args(array = nil)
      if !array.nil?
        @args     = array
        @num_args = array.length
      end
      @args
    end

  end

  def self.define(cmd_name, &init_block)
    class_name = "Command_#{cmd_name.to_s}"
    class_definition = Class.new(Command)
    class_initializer = Proc.new do
      name = cmd_name
      init_block.call
    end
    ::Command.const_set class_name, class_definition
    ::Command.const_get(class_name).send(:define_method, :initialize, class_initializer)
  end

end
