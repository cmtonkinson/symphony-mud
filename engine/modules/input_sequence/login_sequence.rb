module InputSequence

  class LoginSequence < InputSequence

    ###########################################################################
    def initialize
      @name = "login"
    end

    ###########################################################################
    aasm do
      # Initial state.
      state :get_name
      # Path for an existing user.
      state :get_password
      # Path for a new user.
      state :confirm_name
      state :set_password
      state :confirm_password
      # Final states.
      state :login
      state :disconnect

      event :_start do
        transitions from: :_pending, to: :get_name
        after do
          user.print "Please enter your name: "
        end
      end

      event :accept_existing_name do
        transitions from: :get_name, to: :get_password
        after do
          user.print "Please enter your password: "
        end
      end

      event :accept_existing_password do
        transitions from: :get_password, to: :login
      end

      event :accept_new_name do
        transitions from: :get_name, to: :confirm_name
      end

      event :accept_name_confirmation do
        transitions from: :confirm_name, to: :set_password
      end

      event :accept_new_password do
        transitions from: :set_password, to: :confirm_password
      end

      event :accept_password_confirmation do
        transitions from: :confirm_password, to: :login
      end

      event :reject do
        transitions to: :disconnect
      end

    end

    ###########################################################################
    def get_name_handler(input)
      # look up user

      # Fail if:
      #   not exists
      #   already signed in
    end

  end

end
