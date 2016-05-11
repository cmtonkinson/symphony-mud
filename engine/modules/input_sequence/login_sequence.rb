module InputSequence

  class LoginSequence < InputSequence

    ###########################################################################
    def initialize
      super
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
        after do
          user.print "Please confirm your name: "
        end
      end

      event :accept_name_confirmation do
        transitions from: :confirm_name, to: :set_password
        after do
          user.print "Please enter a password: "
        end
      end

      event :accept_new_password do
        transitions from: :set_password, to: :confirm_password
        after do
          user.print "Please confirm your password: "
        end
      end

      event :accept_password_confirmation do
        transitions from: :confirm_password, to: :login
      end

      event :reject_password_confirmation do
        transitions from: :confirm_password, to: :set_password
        after do
          user.print "Passwords do not match; please enter your password again: "
        end
      end

      event :complete do
        transitions to: :login
        after do
          user.print "Welcome!"
          finish
        end
      end

      event :reject do
        transitions to: :disconnect
        after do
          user.client.should_terminate = true
          user.print "Disconnecting."
          finish
        end
      end

    end

    ###########################################################################
    def get_name_handler(input)
      @context.name = input
      if ::Base::Account.exists? @context.name
        @context.existing_name = true
        accept_existing_name
      else
        @context.existing_name = false
        accept_new_name
      end
    end

    ###########################################################################
    def get_password_handler(input)
      @context.password = input
      if ::Base::Account.authenticate @context.name, @context.password
        complete
      else
        reject
      end
    end

    ###########################################################################
    def confirm_name_handler(input)
      @context.name_confirmation = input
      if @context.name == @context.name_confirmation
        accept_name_confirmation
      else
        user.puts "That doesn't match the name you entered; let's try again."
        reset
      end
    end

    ###########################################################################
    def set_password_handler(input)
      @context.new_password = input
      accept_new_password
    end

    ###########################################################################
    def confirm_password_handler(input)
      @context.password_confirmation = input
      if ::Base::Account.equal_time_compare @context.new_password, @context.password_confirmation
        ::Base::Account.add @context.name, @context.new_password
        complete
      else
        reject_password_confirmation
      end
    end

  end

end
