module Command

  class Operator

    ###########################################################################
    def initialize(client, command_set)
      @state       = {}
      @last_input  = ""
      @client      = client
      @command     = nil
      @command_set = command_set
    end

    ###########################################################################
    def handle(input)
      Symphony.logger.everything "Operator::handle('#{input}')"
      cmd_name, _, rest = input.partition " "

      @command = @command_set.find { |c| c.get_name == cmd_name }
      return if @command.nil?

      context = parse rest
      return if context.nil?

      @command.perform @client, context, rest
    end

    ###########################################################################
    def parse(input_string)
      @command.usages.each do |usage|
        Symphony.logger.everything "  Considering Usage #{usage}"
        context = usage.try_parse input_string
        return context if !context.nil?
      end
      return nil
    end

  end

end
