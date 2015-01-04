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
      @formats  = []
      @context  = nil
      instance_eval &block if block_given?
    end

    # Heart and soul of the command processing subsystem.
    def perform(user, input_string)
      # These variables will be available to the exec block as instance variables. See arguments.rb
      # and Command#format for background on why we're indirecting with the STATE_KEYS Hash.
      @context = {
        STATE_KEYS[:user]         => user,
        STATE_KEYS[:input_string] => input_string,
      }
      # Set instance variables for each parsed argument.
      parse(input_string).each { |name,data| @context[name] = data }
      # Pass control to the Command exec Proc.
      @exec.call
    end

    # Parse the given input according to the longest-matching format.
    def parse(input)
      {}
    end

    # This enables us to access anything in the @context Hash with a simple method call inside the
    # Command exec blocks. Said another way, the only access we allow through method_missing is to
    # return values out of the @context Hash for Command execution.
    def method_missing(meth, *args, &block)
      return @context[meth] if @context.keys.include? meth
      super
    end

    # Public getters.
    def get_name;   @name;  end
    def get_level;  @level; end
    def get_group;  @group; end
    def get_exec;   @exec;  end

    private

    ################################################################################################
    # These "private" setters are designed for the Command metaprogramming interface.
    ################################################################################################
    def name(name)
      @name  = name.to_s
    end

    def level(level)
      @level = level.to_i
    end

    def group(group)
      @group = group.to_sym
    end

    def exec(&exec)
      @exec  = exec
    end

    def format(*args)
      arg_names = Set.new
      arg_set   = {}
      # Iterate through the given arguments to get them all into the form `[:name, :type]` since we
      # allow them to be specifed as simply `:name` (in which case `type` defaults to :token.
      args.each do |k,v|
        arg_name = k.to_sym
        # All argument names must be unique, and cannot shadow auto-generated Command variables.
        if arg_names.include? arg_name
          raise ArgumentError.new I18n.t('command.duplicate_name', name: arg_name)
        elsif STATE_KEYS.keys.include? arg_name
          raise ArgumentError.new I18n.t('command.state_key_overlap', name: arg_name)
        else
          arg_names << arg_name
        end
        # Default argument type is token.
        arg_type = v.nil? ? :token : v.to_sym
        if !ARGUMENT_TYPES.keys.include? arg_type
          raise ArgumentError.new I18n.t('command.invalid_type', type: arg_type)
        end
        # At this point we're safe to proceed.
        arg_set[arg_name] = arg_type
      end
      # Keep the formats list sorted by number of formal arguments descending. This is necessary
      # because when we attempt to parse command line mumbo jumbo if we don't check the longest (in
      # terms of number of formal arguments) formats first, it's possible they could get shadowed
      # by shorter formats and create a short circuit in the parser.
      @formats << arg_set
      @formats.sort_by! { |f| -f.length }
    end

  end

  ##################################################################################################
  # Command::define is the metaprogramming interface which allows convenient definition of new
  # commands.
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
