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
      state :get_passphrase
      # Path for a new user.
      state :confirm_name
      state :set_passphrase
      state :confirm_passphrase
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
        transitions from: :get_name, to: :get_passphrase
        after do
          user.print "Please enter your passphrase: "
        end
      end

      event :accept_existing_passphrase do
        transitions from: :get_passphrase, to: :login
      end

      event :accept_new_name do
        transitions from: :get_name, to: :confirm_name
        after do
          user.print "Please confirm your name: "
        end
      end

      event :accept_name_confirmation do
        transitions from: :confirm_name, to: :set_passphrase
        after do
          user.print "Please enter a passphrase: "
        end
      end

      event :accept_new_passphrase do
        transitions from: :set_passphrase, to: :confirm_passphrase
        after do
          user.print "Please confirm your passphrase: "
        end
      end

      event :accept_passphrase_confirmation do
        transitions from: :confirm_passphrase, to: :login
      end

      event :reject_passphrase_confirmation do
        transitions from: :confirm_passphrase, to: :set_passphrase
        after do
          user.print "Passwords do not match; please enter your passphrase again: "
        end
      end

      event :complete do
        transitions to: :login
        after do
          case @context.account_type
          when :new
            user.print "Welcome!"
            user.avatar = Realm::Avatar.new @context.avatar_id, @context.name
            user.avatar.room = $world.find_room Realm::World::THE_VOID_ID
            Realm::Scribe.save user.avatar
            finish
          when :existing
            user.print "Welcome back!"
            user.avatar = Realm::Scribe.load Realm::Avatar, @context.avatar_id
            finish
          end
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
    def get_passphrase_handler(input)
      @context.passphrase = input
      if @context.avatar_id = ::Base::Account.authenticate(@context.name, @context.passphrase)
        @context.account_type = :existing
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
    def set_passphrase_handler(input)
      @context.new_passphrase = input
      accept_new_passphrase
    end

    ###########################################################################
    def confirm_passphrase_handler(input)
      @context.passphrase_confirmation = input
      if ::Base::Account.equal_time_compare @context.new_passphrase, @context.passphrase_confirmation
        @context.account_type = :new
        @context.avatar_id    = SecureRandom.uuid
        ::Base::Account.add @context.name, @context.avatar_id, @context.new_passphrase
        complete
      else
        reject_passphrase_confirmation
      end
    end

  end

end
