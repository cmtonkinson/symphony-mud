module Command

  class Operator
    attr_reader :command_set, :set_name

    ###########################################################################
    def initialize(user, command_set_name = :global)
      @last_input  = '' # TODO: implement this
      @user        = user
      @command     = nil
      @set_name    = command_set_name
      @command_set = ::Command.configuration.command_sets[@set_name]
    end

    ###########################################################################
    def handle(input)
      @command          = nil
      cmd_name, _, rest = input.partition ' '
      being             = @user.avatar

      Symphony.logger.trace "Handling '#{input}' in :#{set_name} for #{being.fqid_str}"

      # If the input was a single character, is it a shortcut?
      if cmd_name.length == 1
        @command = @command_set.find { |c| c.get_shortcut == cmd_name }
      end

      # Try to find by exact name.
      if @command.nil?
        @command = @command_set.find { |c| c.get_name == cmd_name }
      end

      # Try to find by prefix matches.
      if @command.nil?
        @command = @command_set.find { |c| c.get_name.index(cmd_name) === 0 }
      end

      # Couldn't find a Command for this input string.
      return false if @command.nil?

      # We found a Command; can we invoke it?
      being.puts I18n.t('command.operator.err_low_level') and return true if @command.get_level > being.level

      # We found a Command, so let's see if the usage is correct.
      context = parse rest

      # If a context for execution wasn't created, that means the Command was
      # invoked improperly, so we recurse to provide the help topic for the
      # requested Command.
      if context.nil?
        being.puts I18n.t('command.operator.err_invocation')
        handle "help #{cmd_name}"
        return true
      end

      @command.perform being, context, rest
      return true
    end

    ###########################################################################
    def parse(input_string)
      @command.usages.each do |usage|
        #Symphony.logger.debug "  Considering Usage #{usage}"
        context = usage.try_parse input_string
        return context if !context.nil?
      end
      return nil
    end
  end

end
