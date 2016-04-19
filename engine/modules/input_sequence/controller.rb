module InputSequence

  class Controller
    attr_reader :user, :sequence

    ###########################################################################
    def initialize(user, sequence)
      @user     = user
      @sequence = sequence

      sequence.user = user
      sequence._start
    end

    ###########################################################################
    def handle(input)
      Symphony.logger.everything "InputSequence::Controller#handle('#{input}')"
      @sequence.handle input
    end

  end

end

