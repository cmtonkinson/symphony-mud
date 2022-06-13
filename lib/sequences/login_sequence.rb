module InputSequence
  class LoginSequence < InputSequence
    FIRST_QUESTION = :race
    SETUP = {
      race: {
        options: -> { ::Talent::Anatomy.anatomies.map { |a| a::NAME }.sort },
        prompt: 'What is your race? Options are %: '.freeze,
        reject: 'Horror! You would be unrecognizable! Options are %: '.freeze,
        accept: 'Excellent! And a heroic % you shall be!'.freeze,
        to: :gender
      },
      gender: {
        options: -> { ::Realm::Gender.genders.sort },
        prompt: 'What is your gender? Options are %: '.freeze,
        reject: 'Let us clear up that confusion... your options are %: '.freeze,
        accept: '% and proud!'.freeze,
        to: :hand
      },
      hand: {
        options: -> { %w[left right] },
        prompt: 'Please choose a dominant hand (%): '.freeze,
        reject: 'Unfortunately you will have to choose (%): '.freeze,
        accept: 'Henceforth, you shall be %-handed.'.freeze,
        to: :height
      },
      height: {
        options: -> { %w[tiny short average tall huge] },
        prompt: 'And how tall are you? (%): '.freeze,
        reject: 'This is important, please choose from among %: '.freeze,
        accept: 'A hero of % stature! Of course!.'.freeze,
        to: :weight
      },
      weight: {
        options: -> { %w[bony thin average heavy obese] },
        prompt: 'What is your weight? (%): '.freeze,
        reject: 'Unless you are a ghost, you have mass. Please choose %: '.freeze,
        accept: 'A % traveler of the realm, then!'.freeze,
        to: :klass
      },
      klass: {
        options: -> { ::Talent::Klass.klasses.map { |k| k::NAME }.sort },
        prompt: 'Select carefully your class (%): '.freeze,
        reject: 'Let us be serious. Your options are (%): '.freeze,
        accept: 'The mightiest % of the epoch!'.freeze,
        to: :finished
      }
    }.freeze

    def initialize
      super
      @name = 'login'
    end

    def interp(key, str, val = nil)
      replacement = val || SETUP[key][:options].call.join(', ')
      SETUP[key][str].sub '%', replacement
    end

    # rubocop:disable Metrics/BlockLength
    aasm do
      # Initial state.
      state :get_name
      # Path for an existing user.
      state :get_passphrase
      # Path for a new user.
      state :confirm_name
      state :set_passphrase
      state :confirm_passphrase

      # rubocop:disable Security/Eval
      # rubocop:disable Style/EvalWithLocation
      SETUP.each_key { |key| eval("state :get_#{key}") }
      # rubocop:enable Security/Eval
      # rubocop:enable Style/EvalWithLocation

      # Yes 'get_finished' is a stupd state name, but it makes the metaprogramming
      # look cleaner if we don't have to break the pattern of state/handler names
      # during the final part of the configurable sequence.
      state :get_finished

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
          user.client.telnet.echo_off
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
        transitions from: :confirm_passphrase, to: :"get_#{FIRST_QUESTION}"
        after do
          user.print interp(FIRST_QUESTION, :prompt)
        end
      end

      event :reject_passphrase_confirmation do
        transitions from: :confirm_passphrase, to: :set_passphrase
        after do
          user.print 'Passwords do not match; please enter your passphrase again: '
        end
      end

      # Configure event handlers of the generic form:
      #
      # event :accept_gender do
      #   transitions from: :get_gender, to: :get_hand
      #   after do
      #     user.print interp(:hand, :prompt)
      #   end
      # end
      SETUP.each do |key, opts|
        accept_event = "
          event :accept_#{key} do
            transitions from: :get_#{key}, to: :get_#{opts[:to]}
            after do
              if SETUP.key?(:#{opts[:to]})
                user.print interp(:#{opts[:to]}, :prompt)
              else
                @context.account_type = :new
                complete
              end
            end
          end"
        # puts accept_event # for debugging

        # rubocop:disable Security/Eval
        eval(accept_event)
        # rubocop:enable Security/Eval
      end

      event :accept_finished do
        transitions from: :"get_#{SETUP.find { |_, v| v[:to] == :finished }[0]}", to: :get_finished
      end

      event :complete do
        transitions to: :login
        after do
          user.client.telnet.echo_on
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

    def get_passphrase_handler(input)
      @context.passphrase = input
      if (@context.avatar_id = ::Base::Account.authenticate(@context.name, @context.passphrase))
        @context.account_type = :existing
        complete
      else
        reject
      end
    end

    def confirm_name_handler(input)
      @context.name_confirmation = input
      if @context.name == @context.name_confirmation
        accept_name_confirmation
      else
        user.puts "That doesn't match the name you entered; let's try again."
        reset
      end
    end

    # rubocop:disable Naming/AccessorMethodName
    def set_passphrase_handler(input)
      @context.new_passphrase = input
      accept_new_passphrase
    end
    # rubocop:enable Naming/AccessorMethodName

    def confirm_passphrase_handler(input)
      @context.passphrase_confirmation = input
      if ::Base::Account.equal_time_compare @context.new_passphrase, @context.passphrase_confirmation
        accept_passphrase_confirmation
      else
        reject_passphrase_confirmation
      end
    end

    # Configure methods of the form:
    #
    # def get_height_handler(input)
    #   @context.height = input
    #   if %w[tiny short average tall huge].include? @context.height
    #     accept_height
    #   else
    #     puts "Not a valid height"
    #   end
    # end
    SETUP.each do |key, opts|
      define_method :"get_#{key}_handler" do |input|
        @context.public_send :"#{key}=", input.downcase
        if opts[:options].call.map(&:downcase).include? @context.public_send key
          user.puts interp(key, :accept, @context.public_send(key))
          public_send :"accept_#{key}"
        else
          user.puts interp(key, :reject)
        end
      end
    end

    def new_avatar_setup_from_context(avatar)
      ::Talent.const_get(@context.race.capitalize).new(avatar)
      ::Talent.const_get(@context.klass.capitalize).new(avatar)
      avatar.gender = @context.gender
      avatar.hand = @context.hand
      avatar.height = SETUP[:height][:options].call.index(@context.height) + 1
      avatar.weight = SETUP[:weight][:options].call.index(@context.weight) + 1

      Logistics::Integrator.stitch avatar, $world.find_room(Realm::World::THE_VOID_ID)

      ::Base::Account.add @context.name, @context.avatar_id, @context.new_passphrase
      Logistics::Scribe.save avatar
      Symphony.logger.info "New account #{avatar.fqid_str}"
      avatar.room.puts I18n.t('universe.room_announce_newbie'), avatar
    end
  end
  # rubocop:enable Metrics/BlockLength
end
