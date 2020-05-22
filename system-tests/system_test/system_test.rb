module SystemTest
  class SystemTest

    ###########################################################################
    def initialize
    end

    ###########################################################################
    def setup(&block)
      @id         = nil
      @parent_id  = nil
      @before     = nil
      @after      = nil
      @test       = nil
      instance_eval(&block) if block_given?
    end

    ###########################################################################
    def conduct_test(port, state_path)
      begin
        raise 'test undefined' if @test.nil?

        @before.call unless @before.nil?
        @test.call
        @after.call unless @after.nil?
      rescue StandardError => e
        Symphony.logger.error "Test #{id} raised -- #{e}"
        e.backtrace.each { |line| Symphony.logger.error line.sub(Symphony.root.to_s, '') }
      end
    end

    ###########################################################################
    # rubocop:disable Naming/AccessorMethodName
    def get_id;         @id;        end
    def get_parent_id;  @parent_id; end
    # rubocop:enable Naming/AccessorMethodName

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

