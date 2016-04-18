module InputSequence

  class LoginSequence < InputSequence

    ###########################################################################
    def initialize
      @name = "login"
    end

    ###########################################################################
    aasm do
      # Initial state
      state :get_name, initial: true
      # Path if the avatar name is recognized
      state :get_password
      # Path if the avatar name is not recognized
      state :confirm_name
      state :set_password
      state :confirm_password
      # Final states
      state :login
      state :disconnect

      after_all_transitions :log_status_change

      def log_status_change
        puts "changing from #{aasm.from_stat} to #{aasm.to_state} (event: #{aasm.current_event})"
      end

    end

  end

end
