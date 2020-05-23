module SystemTest
  class SystemTest
    attr_accessor :status, :result

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
      @account_name = nil
      @status       = :pending
      @result       = nil
      instance_eval(&block) if block_given?
    end

    ###########################################################################
    def conduct_test(context)
      @context = context

      begin
        raise 'test undefined' if @test.nil?

        login_or_create if @account_name

        @before.call unless @before.nil?
        @test.call
        @after.call unless @after.nil?
        @status = :pass
      rescue StandardError => e
        @status = :fail
        @result = e
      end
    end

    ###########################################################################
    def login_or_create
      result = @context.client.command @account_name
      if /confirm/i.match result
        create
      else
        login
      end
    end

    def login
      @context.client.command @context.client.password
    end

    def create
      @context.client.command @account_name
      @context.client.command @context.client.password
      @context.client.command @context.client.password
    end

    ###########################################################################
    # rubocop:disable Naming/AccessorMethodName
    def get_id;           @id;           end
    def get_parent_id;    @parent_id;    end
    def get_account_name; @account_name; end
    # rubocop:enable Naming/AccessorMethodName

    ###########################################################################
    # Automatically proxy messages to the @context Hash, the same way we do
    # in Command::Command.
    def method_missing(meth, *args, &block)
      if @context.respond_to? meth
        return @context.send meth
      else
        begin
          return super
        rescue NameError => e
          raise e if e.name == :execute
          return nil
        end
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
      @account_name = account_name
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

