module Base
  class User
    attr_accessor :client, :avatar, :operators

    ###########################################################################
    def initialize
      @client    = nil
      @avatar    = nil
      @operators = Util::Stack.new
    end

    ###########################################################################
    def bind_client(client, login = true)
      # Cross-reference the User and Client.
      @client      = client
      @client.user = self

      # Push the default Operator.
      operators.push Command::Operator.new self

      # Assuming this is a new login, Push the LoginSequence.
      operators.push InputSequence::Controller.new self, InputSequence::LoginSequence.new if login
    end

    ###########################################################################
    def bind_avatar(avatar)
      @avatar      = avatar
      @avatar.user = self
    end

    ###########################################################################
    # Proxy Client methods.
    %i[print puts].each do |meth|
      define_method meth do |message|
        client.send meth, message
      end
    end

    ###########################################################################
    def handle(input)
      handled = false
      # If the user just hits Enter (with or without whitespace), send them
      # a new prompt. This is useful if they're just waiting on their stats
      # to heal.
      if /\A\s*\z/.match?(input)
        @avatar.force_output!
        return false
      end

      # Enumerate operators (in order) to find one able to handle the input.
      operators.each do |op|
        if op.handle(input) == true
          handled = true
          break
        end
      end
      return if handled

      # Nope!
      puts I18n.t 'command.operator.err_not_found'
    end
  end
end
