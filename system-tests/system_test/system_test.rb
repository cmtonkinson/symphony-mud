module SystemTest
  class SystemTest
    attr_accessor :accounts, :status, :result, :assertions, :output_polling

    ###########################################################################
    def initialize; end

    ###########################################################################
    def setup(&block)
      @id                = nil
      @parent_id         = nil
      @before            = nil
      @after             = nil
      @test              = nil
      @accounts          = []
      @account_opts      = {}
      @last_used_account = nil
      @output_polling    = true
      @status            = :pending
      @assertions        = 0
      @result            = nil
      @start_time        = nil
      @end_time          = nil
      instance_eval(&block) if block_given?
    end

    ###########################################################################
    def conduct_test(context)
      @context = context

      begin
        raise 'test undefined' if @test.nil?

        @start_time = Time.now

        @accounts.each { |a| login_or_create a }

        @before&.call
        @test.call
        @after&.call
        @status = :pass unless @status == :fail

        @end_time = Time.now
      rescue StandardError => e
        @end_time = Time.now
        @status = :fail

        message = ("\n    %s\n    " % e.message) + e.backtrace
                                                    .select! { |l| l.include? $ROOT_DIR.to_s }
                                                    .each { |l| l.sub!($ROOT_DIR.to_s, '') }
                                                    .join("\n    ")
        @result = message
      end
    end

    ###########################################################################
    def expect(e, msg = nil)
      unless @last_used_account
        raise 'SystemTest#expect() called without first send a command through the shortcut interface.'
      end

      if e.is_a?(String) || e.is_a?(Regexp)
        expect_last acm(@last_used_account), e, msg
      elsif e.is_a?(Array)
        e.each { |term| expect_last acm(@last_used_account), term, msg }
      else
        raise 'SystemTest#expect() only accepts String, Regexp, or Array of same'
      end
    end

    ###########################################################################
    def n_expect(e, msg = nil)
      unless @last_used_account
        raise 'SystemTest#n_expect() called without first send a command through the shortcut interface.'
      end

      if e.is_a?(String) || e.is_a?(Regexp)
        n_expect_last acm(@last_used_account), e, msg
      elsif e.is_a?(Array)
        e.each { |term| n_expect_last acm(@last_used_account), term, msg }
      else
        raise 'SystemTest#n_expect() only accepts String, Regexp, or Array of same'
      end
    end

    ###########################################################################
    def expect_last(client, e, msg = nil)
      return if can_find(client, -1, e)

      @status = :fail
      @result = not_found_error e, msg
    end

    ###########################################################################
    def expect_any(client, e, msg = nil)
      return if can_find(client, nil, e)

      @status = :fail
      @result = not_found_error e, msg
    end

    ###########################################################################
    def n_expect_last(client, e, msg = nil)
      return unless can_find(client, -1, e)

      @status = :fail
      @result = found_error e, msg
    end

    ###########################################################################
    def n_expect_any(client, e, msg = nil)
      return unless can_find(client, nil, e)

      @status = :fail
      @result = found_error e, msg
    end

    ###########################################################################
    def can_find(client, range, e)
      @assertions += 1

      if range.nil?
        corpus = client.buffer.join "\n"
      else
        corpus = client.buffer[range]
        corpus = corpus.join("\n") if corpus.is_a? Array
      end

      case e
      when String then corpus.include?(e)
      when Regexp then e.match(corpus)
      else raise 'SystemTest#can_find requires a String or Regexp'
      end
    end

    ###########################################################################
    def not_found_error(e, msg = nil)
      case e
      when String then "Did not find substring '#{e}'%s" % (msg ? '  ' + msg : '')
      when Regexp then "No match found for '#{e}'%s" % (msg ? '  ' + msg : '')
      else raise 'Bad param'
      end
    end

    ###########################################################################
    def found_error(e, _msg = nil)
      case e
      when String then "Found substring '#{e}'"
      when Regexp then "Matched '#{e}'"
      else raise 'Bad param'
      end
    end

    ###########################################################################
    def pass(str = nil)
      @status = :pass
      @result = str || 'Pass method called'
    end

    ###########################################################################
    def fail(str = nil)
      @status = :fail
      @result = str || 'Fail method called'
    end

    ###########################################################################
    def run_time
      '%.3fs' % (@end_time - @start_time).to_f
    end

    ###########################################################################
    def account_client_method(account_name)
      @last_used_account = account_name.to_s
      @context.send (account_name.to_s + '_client').to_sym
    end
    alias acm account_client_method

    def login_or_create(account_name)
      client = acm account_name
      result = client.command account_name
      if /confirm/i.match result
        create account_name
      else
        login account_name
      end
    end

    def login(account_name)
      client = acm account_name
      client.command client.password
    end

    def create(account_name)
      client = acm account_name
      client.command account_name
      client.command client.password
      client.command client.password

      opt = get_creation_opts account_name
      client.command opt[:race]
      client.command opt[:gender]
      client.command opt[:hand]
      client.command opt[:height]
      client.command opt[:weight]
      client.command opt[:klass]

      # For the sake of log verbosity, always show the summary after creating
      # a new account.
      client.command 'summary'
    end

    def get_creation_opts(account_name)
      default_creation_opts.merge @account_opts[account_name.to_s]
    end

    def default_creation_opts
      {
        race: 'human',
        gender: 'female',
        hand: 'right',
        height: 'average',
        weight: 'average',
        klass: 'warrior'
      }
    end

    ###########################################################################
    # rubocop:disable Naming/AccessorMethodName
    def get_id
      @id
    end

    def get_parent_id
      @parent_id
    end

    def get_account_name
      @account_name
    end
    # rubocop:enable Naming/AccessorMethodName

    ###########################################################################
    # This convoluted garbage does a few things, all aimed at easing the
    # convenience of the system test DSL.
    def method_missing(meth, *args, &block) # rubocop:disable Style/MissingRespondToMissing
      # The first form simply looks to proxy the method directly to the
      # @context object.
      if @context.respond_to? meth
        @context.send(meth, *args, &block)
      # The second form assumes that you either want to send a command through
      # a given connection and you'll reference it by name (e.g. you want
      # to type `ogre "kill elf"` in your test and have the command "kill elf"
      # be send through the connection called :ogre
      # OR
      # You want to return the Client connection object itself, in which case
      # you'd call the method with no params (e.g. `ogre`)
      elsif @accounts.include?(meth.to_s)
        retval = if args.size == 1 && args[0].is_a?(String)
                   acm(meth).command args[0]
                 else
                   acm(meth)
                 end

        # The underlying Net::Telnet client won't read new data from the
        # connection until/unless waitfor() is called. That means if something
        # done in one connection causes output to another, either we won't
        # get it at all, or else the sequencing of inputs and outputs in the
        # client log will be out of order.
        #
        # In other words, if we don't poll for new data on every connection
        # after any connection does anything, the client logs will show
        # output late, if at all.
        if output_polling
          @accounts.reject { |a| a == meth.to_s }.each do |a|
            # Don't use the acm method or it'll make @last_used_account wrong.
            @context.send("#{a}_client".to_sym).poll
          end
        end

        retval
      # The fallback is the default behavior of method_missing.
      else
        super
      end
    end

    private

    ###########################################################################
    def id(id)
      @id = id.to_s
    end

    def from(parent_id)
      @parent_id = parent_id.to_s
    end

    def before(&before)
      @before = before
    end

    def after(&after)
      @after = after
    end

    def test(&test)
      @test = test
    end

    def with(account_name, opts = {})
      raise "You cannot use an account named 'server' in a SystemTest" if account_name == :server

      @accounts << account_name.to_s
      @account_opts[account_name.to_s] = opts
    end
  end

  ###########################################################################
  def self.define(test_id, &init_block)
    class_name = "SystemTest_#{test_id}"
    class_definition = Class.new(SystemTest) do
      define_method :initialize do
        method(:setup).call(&init_block)
        method(:id).call test_id
      end
    end
    ::SystemTest.const_set class_name, class_definition
    Configuration.configure { |conf| conf.register_test(::SystemTest.const_get(class_name).new) }
  end
end
