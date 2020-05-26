module SystemTest
  class SystemTest
    attr_accessor :accounts, :status, :result, :assertions

    ###########################################################################
    def initialize
    end

    ###########################################################################
    def setup(&block)
      @id           = nil
      @parent_id    = nil
      @before       = nil
      @after        = nil
      @test         = nil
      @accounts     = []
      @last_used_account = nil
      @status       = :pending
      @assertions   = 0
      @result       = nil
      @start_time   = nil
      @end_time     = nil
      instance_eval(&block) if block_given?
    end

    ###########################################################################
    def conduct_test(context)
      @context = context

      begin
        raise 'test undefined' if @test.nil?

        @start_time = Time.now

        @accounts.each { |a| login_or_create a }

        @before.call unless @before.nil?
        @test.call
        @after.call unless @after.nil?
        @status = :pass unless @status == :fail

        @end_time = Time.now
      rescue StandardError => e
        @end_time = Time.now
        @status = :fail

        message = ("\n    %s\n    " % e.message) + e.backtrace
          .reject! { |l| !l.include? $ROOT_DIR.to_s }
          .each { |l| l.sub!($ROOT_DIR.to_s, '') }
          .join("\n    ")
        @result = message
      end
    end

    ###########################################################################
    def expect(e)
      raise "SystemTest#expect() called without first send a command through the shortcut interface." unless @last_used_account
      expect_last acm(@last_used_account), e
    end

    ###########################################################################
    def n_expect(e)
      raise "SystemTest#n_expect() called without first send a command through the shortcut interface." unless @last_used_account
      n_expect_last acm(@last_used_account), e
    end

    ###########################################################################
    def expect_last(client, e)
      if !can_find(client, -1, e)
        @status = :fail
        @result = not_found_error e
      end
    end

    ###########################################################################
    def expect_any(client, e)
      if !can_find(client, nil, e)
        @status = :fail
        @result = not_found_error e
      end
    end

    ###########################################################################
    def n_expect_last(client, e)
      if can_find(client, -1, e)
        @status = :fail
        @result = found_error e
      end
    end

    ###########################################################################
    def n_expect_any(client, e)
      if can_find(client, nil, e)
        @status = :fail
        @result = found_error e
      end
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

      return case e
      when String then corpus.include?(e)
      when Regexp then e.match(corpus)
      else raise "SystemTest#can_find requires a String or Regexp"
      end
    end

    ###########################################################################
    def not_found_error(e)
      case e
      when String then "Did not find substring '#{e}'"
      when Regexp then "No match found for '#{e}'"
      else raise "Bad param"
      end
    end

    ###########################################################################
    def found_error(e)
      case e
      when String then "Found substring '#{e}'"
      when Regexp then "Matched '#{e}'"
      else raise "Bad param"
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
      @last_used_account = account_name
      @context.send((account_name + "_client").to_sym)
    end
    alias_method :acm, :account_client_method

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
      client.command 'human'
      client.command 'female'
      client.command 'left'
      client.command 'average'
      client.command 'average'
      client.command 'warrior'
    end

    ###########################################################################
    # rubocop:disable Naming/AccessorMethodName
    def get_id;           @id;           end
    def get_parent_id;    @parent_id;    end
    def get_account_name; @account_name; end
    # rubocop:enable Naming/AccessorMethodName

    ###########################################################################
    # This convoluted garbage does a few things, all aimed at easing the
    # convenience of the system test DSL.
    def method_missing(meth, *args, &block)
      # The first form simply looks to proxy the method directly to the
      # @context object.
      if @context.respond_to? meth
        return @context.send(meth, *args, &block)
      # The second form assumes that you either want to send a command through
      # a given connection and you'll reference it by name (e.g. you want
      # to type `ogre "kill elf"` in your test and have the command "kill elf"
      # be send through the connection called :ogre
      # OR
      # You want to return the Client connection object itself, in which case
      # you'd call the method with no params (e.g. `ogre`)
      elsif @accounts.include?(meth.to_s)
        if args.size == 1 && args[0].is_a?(String)
          return acm(meth.to_s).command args[0]
        else
          return acm(meth.to_s)
        end
      # The fallback is the default behavior of method_missing.
      else
        return super
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

    def with(account_name)
      @accounts << account_name
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

