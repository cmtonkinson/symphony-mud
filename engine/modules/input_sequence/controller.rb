module InputSequence

  class Controller

    ###########################################################################
    def initialize(client, sequence)
      @client   = client
      @sequence = sequence
    end

    ###########################################################################
    def handle(input)
      Symphony.logger.everything "InputSequence::Controller#handle('#{input}')"
    end

  end

end
