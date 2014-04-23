module Command

  class Command

    def initialize
      name ""
      level 1
      exec nil
      @args     = []
      @num_args = 0
    end

    def <=> (other)
      self.get_name <=> other.get_name
    end

    def setup(&block)
      instance_eval &block
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

    def get_name; @name; end
    def get_level; @level; end
    def get_exec; @exec; end

    private

    ################################################################################################
    # These methods are designed for the Command DSL.
    ################################################################################################
    def name(name); @name = name; end
    def level(level); @level = level; end
    def exec(&exec); @exec = exec; end

  end

  def self.define(cmd_name, &init_block)
    class_name = "Command_#{cmd_name.to_s}"
    class_definition = Class.new(Command) do
      define_method :initialize do
        method(:name).call cmd_name.to_s
        method(:setup).call &init_block
      end
    end
    ::Command.const_set class_name, class_definition
  end

end
