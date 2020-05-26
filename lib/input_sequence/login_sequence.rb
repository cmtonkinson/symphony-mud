module InputSequence
  class LoginSequence < InputSequence

    # Copypasta from authoritative sources of truth elsewhere in the codebase.
    # Really not the best method for exposing this information.
    # TODO - clean this crap up!
    RACE_OPTIONS   = %w(human)
    GENDER_OPTIONS = %w(male female neuter)
    HAND_OPTIONS   = %w(left right)
    HEIGHT_OPTIONS = %w(tiny short average tall huge)
    WEIGHT_OPTIONS = %w(bony thin average heavy obese)
    KLASS_OPTIONS  = %w(warrior mage cleric rogue)

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
      state :get_race
      state :get_gender
      state :get_hand
      state :get_height
      state :get_weight
      state :get_klass
      # Final states.
      state :login
      state :disconnect

      event :_start do
        transitions from: :_pending, to: :get_name
        after do
          user.puts 'You have connected to the server.'
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
        transitions from: :confirm_passphrase, to: :get_race
        after do
          user.print "What is your race? Options are [#{RACE_OPTIONS.join ', '}]: "
        end
      end

      event :reject_passphrase_confirmation do
        transitions from: :confirm_passphrase, to: :set_passphrase
        after do
          user.print 'Passwords do not match; please enter your passphrase again: '
        end
      end

      event :accept_race do
        transitions from: :get_race, to: :get_gender
        after do
          user.print "What is your gender? Options are [#{GENDER_OPTIONS.join ', '}]: "
        end
      end

      event :accept_gender do
        transitions from: :get_gender, to: :get_hand
        after do
          user.print "Which is your dominant hand? Options are [#{HAND_OPTIONS.join ', '}]: "
        end
      end

      event :accept_hand do
        transitions from: :get_hand, to: :get_height
        after do
          user.print "How tall are you? Options are [#{HEIGHT_OPTIONS.join ', '}]: "
        end
      end

      event :accept_height do
        transitions from: :get_height, to: :get_weight
        after do
          user.print "How heavy are you? Options are [#{WEIGHT_OPTIONS.join ', '}]: "
        end
      end

      event :accept_weight do
        transitions from: :get_weight, to: :get_klass
        after do
          user.print "What is your class? Options are [#{KLASS_OPTIONS.join ', '}]: "
        end
      end

      event :complete do
        transitions to: :login
        after do
          case @context.account_type
          when :new
            user.puts 'Welcome!'
            @context.avatar_id = SecureRandom.uuid
            avatar = Realm::Avatar.new @context.avatar_id, name: @context.name
            user.bind_avatar avatar
            new_avatar_setup_from_context avatar
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
      if (@context.avatar_id = ::Base::Account.authenticate(@context.name, @context.passphrase))
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
    # rubocop:disable Naming/AccessorMethodName
    def set_passphrase_handler(input)
      @context.new_passphrase = input
      accept_new_passphrase
    end
    # rubocop:enable Naming/AccessorMethodName

    ###########################################################################
    def confirm_passphrase_handler(input)
      @context.passphrase_confirmation = input
      if ::Base::Account.equal_time_compare @context.new_passphrase, @context.passphrase_confirmation
       accept_passphrase_confirmation
      else
        reject_passphrase_confirmation
      end
    end

    ###########################################################################
    def get_race_handler(input)
      @context.race = input
      if RACE_OPTIONS.include? @context.race
        accept_race
      else
        puts "That's not a valid choice."
      end
    end

    ###########################################################################
    def get_gender_handler(input)
      @context.gender = input
      if GENDER_OPTIONS.include? @context.gender
        accept_gender
      else
        puts "That's not a valid choice."
      end
    end

    ###########################################################################
    def get_hand_handler(input)
      @context.hand = input
      if HAND_OPTIONS.include? @context.hand
        accept_hand
      else
        puts "That's not a valid choice."
      end
    end

    ###########################################################################
    def get_height_handler(input)
      @context.height = input
      if HEIGHT_OPTIONS.include? @context.height
        accept_height
      else
        puts "That's not a valid choice."
      end
    end

    ###########################################################################
    def get_weight_handler(input)
      @context.weight = input
      if WEIGHT_OPTIONS.include? @context.weight
        accept_weight
      else
        puts "That's not a valid choice."
      end
    end

    ###########################################################################
    def get_klass_handler(input)
      @context.klass = input
      if KLASS_OPTIONS.include? @context.klass
        @context.account_type = :new
        complete
      else
        puts "That's not a valid choice."
      end
    end

    ###########################################################################
    def new_avatar_setup_from_context(avatar)
      avatar.room = $world.find_room Realm::World::THE_VOID_ID
      avatar.anatomy = ::Talent.const_get(@context.race.capitalize).new(avatar)
      avatar.add_enabler avatar.anatomy
      avatar.gender = @context.gender
      avatar.hand = @context.hand
      avatar.height = HEIGHT_OPTIONS.index(@context.height) + 1
      avatar.weight = WEIGHT_OPTIONS.index(@context.weight) + 1
      avatar.klass = ::Talent.const_get(@context.klass.capitalize).new
      avatar.add_enabler avatar.klass
      avatar.room.insert avatar
      avatar.run_hook :vivify

      ::Base::Account.add @context.name, @context.avatar_id, @context.new_passphrase
      Logistics::Scribe.save avatar
      Symphony.logger.info "New account #{avatar.fqid_str}"
      avatar.room.puts I18n.t('universe.room_announce_newbie'), avatar
    end

  end
end

