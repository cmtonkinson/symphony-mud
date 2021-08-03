module Command
  class Operator
    attr_reader :command_set, :set_name

    def initialize(user, command_set_name = :global)
      @last_input  = '' # TODO: implement this
      @user        = user
      @command     = nil
      @set_name    = command_set_name
      @command_set = ::Command.configuration.command_sets[@set_name]
    end

    def handle(input)
      @command          = nil
      being             = @user.avatar
      cmd_name, _, rest = input.partition ' '

      return true if being.nil?

      Symphony.logger.trace "Handling '#{input}' in :#{set_name} for #{being.fqid_str}"

      # If the input was a single character, is it a shortcut?
      @command = @command_set.find { |c| c.get_shortcut == cmd_name } if cmd_name.length == 1

      # Try to find by exact name.
      @command = @command_set.find { |c| c.get_name == cmd_name } if @command.nil?

      # Try to find by prefix matches.
      @command = @command_set.find { |c| c.get_name.index(cmd_name) == 0 } if @command.nil?

      # Couldn't find a Command for this input string.
      return false if @command.nil?

      # We found a Command; is it a skill we have?
      return false if @command.ability? && !being.get_ability(@command.get_name)

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

      Symphony.logger.trace "  performing command '#{@command.get_name}' with arguments '#{rest}' for #{being.fqid_str}"
      @command.perform being, context, rest
      true
    end

    def parse(input_string)
      @command.usages.each do |usage|
        context = usage.try_parse input_string
        return context unless context.nil?
      end
      nil
    end
  end
end
