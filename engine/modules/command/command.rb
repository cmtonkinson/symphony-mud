module Command

  class Command

    def initialize
    end

    def <=> (other)
      self.get_name <=> other.get_name
    end

    # Set sane defaults, and run the `define` block from the command file.
    def setup(&block)
      @name     = "_"
      @level    = 1
      @group    = :global
      @exec     = nil
      @args     = []
      @num_args = 0
      instance_eval &block if block_given?
    end

    # Heart and soul of the command processing subsystem.
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

    # Keep parity between @args and @num_args.
    def args(array = nil)
      if !array.nil?
        @args     = array
        @num_args = array.length
      end
      @args
    end

    # Public getters.
    def get_name; @name; end
    def get_level; @level; end
    def get_group; @group; end
    def get_exec; @exec; end

    private

    # Private setter methods are designed for Command metaprogramming.
    def name(name);   @name  = name.to_s;     end
    def level(level); @level = level.to_i;    end
    def group(group); @group = group.to_sym;  end
    def exec(&exec);  @exec  = exec;          end

  end

  ##################################################################################################
  # Command::define is the metaprogramming interface which allows convenient definition of new
  # commands>
  ##################################################################################################
  def self.define(cmd_name, &init_block)
    class_name = "Command_#{cmd_name.to_s}"
    class_definition = Class.new(Command) do
      define_method :initialize do
        method(:setup).call &init_block
        method(:name).call cmd_name.to_s
      end
    end
    ::Command.const_set class_name, class_definition
    ::Command.configure { |config| config.register_command(::Command.const_get(class_name).new) }
  end

end
