module Command

  class Operator

    attr_reader :command_set

    ###########################################################################
    def initialize(user, command_set_name = :global)
      @last_input  = "" # TODO - implement this
      @user        = user
      @command     = nil
      @command_set = ::Command.configuration.command_sets[command_set_name]
    end

    ###########################################################################
    def handle(input)
      Symphony.logger.absurd "Command::Operator#handle('#{input}')"
      cmd_name, _, rest = input.partition " "

      @command = @command_set.find { |c| c.get_name == cmd_name }
      return if @command.nil?

      context = parse rest

      # If a context for execution wasn't created, that means the Command was
      # invoked improperly, so we recurse to provide the help topic for the
      # requested Command.
      if context.nil?
        handle "help #{cmd_name}"
        return
      end

      @command.perform @user.avatar, context, rest
    end

    ###########################################################################
    def parse(input_string)
      @command.usages.each do |usage|
        Symphony.logger.absurd "  Considering Usage #{usage}"
        context = usage.try_parse input_string
        return context if !context.nil?
      end
      return nil
    end

  end

end
