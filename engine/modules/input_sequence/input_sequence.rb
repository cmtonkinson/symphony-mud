module InputSequence

  class InputSequence
    include AASM

    attr_reader   :name
    attr_accessor :user

    ###########################################################################
    def handle(input)
      _handle input
    end

    #########################################################################
    aasm do
      after_all_transitions :log_status_change
      state :_pending, initial: true
    end

    #########################################################################
    def log_status_change
      str = "transitioning "
      str << "from '#{aasm.from_state}' " if aasm.from_state != nil
      str << "to '#{aasm.to_state}' "
      str << "(on '#{aasm.current_event}')"
      Symphony.logger.debug str
    end

  end

end

