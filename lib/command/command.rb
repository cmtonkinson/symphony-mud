module Command

  class Command
    attr_reader :usages

    ###########################################################################
    def initialize
    end

    ###########################################################################
    def <=> (other)
      @name <=> other.get_name
    end

    ###########################################################################
    # Set sane defaults, and run the `define` block from the command file.
    def setup(&block)
      @name     = '_'
      @shortcut = nil
      @brief    = 'TODO'
      @level    = 1
      @group    = :global
      @exec     = nil
      @usages   = [] # A collection of Usages
      @context  = nil
      instance_eval &block if block_given?
    end

    ###########################################################################
    # Heart and soul of the command processing subsystem.
    def perform(being, context, input_string)
      # Store the new context.
      @context = context

      # Add additional default context variables.
      @context.being      = being
      @context.full_input = input_string

      # Convenience methods.
      @context.define_singleton_method :room, -> { being.room }

      # Pass control to the Command exec Proc.
      begin
        raise '@exec undefined' if @exec.nil?
        @exec.call
      rescue Exception => e
        Symphony.logger.error "Command '#{@name}' with #{input_string.inspect} by '#{being.fqid_str}' raised -- #{e}"
        e.backtrace.each { |line| Symphony.logger.debug line.sub(Symphony.root.to_s, '') }
      end

      # Eagerly purge the context (to be safe).
      @context = nil
    end

    ###########################################################################
    # Since we key all command text translations by the command name, we can
    # provide a convenience method to invoke I18n#t, prefix the command name
    # and pass through the optional arguments.
    def translate(str, *args)
      I18n.t "#{get_group}.#{get_name}.#{str}", *args
    end
    alias_method :t, :translate

    ###########################################################################
    # This enables us to access anything in the @context Hash with a simple method call inside the
    # Command exec blocks. Said another way, the only access we allow through method_missing is to
    # return values out of the @context Hash for Command execution.
    def method_missing(meth, *args, &block)
      if @context.respond_to? meth
        return @context.send meth
      else
        begin
          return super
        rescue NameError => e
          raise e if e.name == :execute
          return nil
        end
      end
    end

    ###########################################################################
    def get_name;     @name;     end
    def get_shortcut; @shortcut; end
    def get_brief;    @brief;    end
    def get_level;    @level;    end
    def get_group;    @group;    end
    def get_exec;     @exec;     end

    private

    ###########################################################################
    # These "private" setters are designed for the Command metaprogramming interface.
    def name(name)
      @name = name.to_s
    end

    def shortcut(shortcut)
      @shortcut = shortcut.to_s
    end

    def brief(brief)
      @brief = brief.to_s
    end

    def level(level)
      @level = level.to_i
    end

    def group(group)
      @group = group.to_sym
    end

    def exec(&exec)
      @exec = exec
    end

    ###########################################################################
    # Keep the usages list sorted by number of formal arguments descending. This is necessary
    # because when we attempt to parse command line mumbo jumbo if we don't check the longest (in
    # terms of number of formal arguments) usages first, it's possible they could get shadowed
    # by shorter usages and create a short circuit in the parser.
    def usage(*argument_list)
      args = Usage.new
      # Can only have one empty usage.
      if argument_list.empty? && @usages.any?(&:empty?)
        raise ArgumentError.new I18n.t 'command.only_one_empty'
      end
      # Add each Argument to the Usage.
      argument_list.each { |defn| args.push Argument.new defn }
      # Add the Usage to the approved formats for the command.
      @usages << args
      @usages.sort_by!(&:sort_method)
    end
  end

  #############################################################################
  # Command::define is the metaprogramming interface which allows convenient definition of new
  # commands.
  def self.define(cmd_name, group_name = :global, &init_block)
    class_name = "Command_#{group_name}_#{cmd_name.to_s}"
    class_definition = Class.new(Command) do
      define_method :initialize do
        method(:setup).call &init_block
        method(:name).call cmd_name.to_s
        method(:group).call group_name.to_s
        method(:brief).call I18n.t("#{get_group}.#{cmd_name}._brief")
      end
    end
    ::Command.const_set class_name, class_definition
    ::Command.configure { |config| config.register_command(::Command.const_get(class_name).new) }
  end

end

