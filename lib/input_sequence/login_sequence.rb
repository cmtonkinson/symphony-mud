module InputSequence
  class LoginSequence < InputSequence
    ###########################################################################
    def initialize
      super
      @name = 'login'
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
          user.print 'Please enter your name: '
        end
      end

      event :accept_existing_name do
        transitions from: :get_name, to: :get_passphrase
        after do
          user.print 'Please enter your passphrase: '
        end
      end

      event :accept_existing_passphrase do
        transitions from: :get_passphrase, to: :login
      end

      event :accept_new_name do
        transitions from: :get_name, to: :confirm_name
        after do
          user.print 'Please confirm your name: '
        end
      end

      event :accept_name_confirmation do
        transitions from: :confirm_name, to: :set_passphrase
        after do
          user.print 'Please enter a passphrase: '
        end
      end

      event :accept_new_passphrase do
        transitions from: :set_passphrase, to: :confirm_passphrase
        after do
          user.print 'Please confirm your passphrase: '
        end
      end

      event :accept_passphrase_confirmation do
        transitions from: :confirm_passphrase, to: :login
      end

      event :reject_passphrase_confirmation do
        transitions from: :confirm_passphrase, to: :set_passphrase
        after do
          user.print 'Passwords do not match; please enter your passphrase again: '
        end
      end

      event :complete do
        transitions to: :login
        after do
          case @context.account_type
          when :new
            user.puts 'Welcome!'
            avatar = Realm::Avatar.new @context.avatar_id, name: @context.name
            user.bind_avatar avatar
            avatar.room = $world.find_room Realm::World::THE_VOID_ID
            avatar.room.insert avatar
            avatar.run_hook :vivify
            avatar.room.puts I18n.t('universe.room_announce_newbie'), avatar
            Symphony.logger.info "New account #{avatar.fqid_str}"
            Logistics::Scribe.save avatar
            avatar.run_hook :on_first_login
          when :existing
            avatar = Logistics::Scribe.load Realm::Avatar, @context.avatar_id
            if avatar
              user.puts 'Welcome back!'
              user.bind_avatar avatar
              avatar.room.puts I18n.t('universe.room_announce_return'), avatar
              avatar.run_hook :vivify
              Symphony.logger.info "#{avatar.fqid_str} logged in"
              avatar.run_hook :on_login
            else
              user.puts '{RSomething went wrong loading your save data; please contact an administrator'
              Symphony.logger.error "Failed to complete login of existing avatar #{@context.avatar_id}"
              reject
            end
          else
            Util.argerr "incomplete transition to :complete; #{@context.account_type} unrecognized"
          end
          user.avatar.cmd 'look' if avatar
          finish
        end
      end

      event :reject do
        transitions to: :disconnect
        after do
          user.client.should_terminate = true
          user.puts 'Disconnecting.'
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
