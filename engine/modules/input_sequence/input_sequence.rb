module InputSequence

  class InputSequence
    include AASM

    attr_reader   :name
    attr_accessor :user

    ###########################################################################
    def handle(input)
      Symphony.logger.everything "#{self.class.name} in state '#{aasm.current_state}' handling '#{input}'"
      send "#{aasm.current_state}_handler", input
    end

    #########################################################################
    aasm do
      after_all_transitions :log_status_change
      state :_pending, initial: true
    end

    #########################################################################
    def log_status_change
      str = self.class.name
      str << " transitioning "
      str << "from '#{aasm.from_state}' " if aasm.from_state != nil
      str << "to '#{aasm.to_state}' "
      str << "(on '#{aasm.current_event}')"
      Symphony.logger.debug str
    end

  end

end
