module Talent

  #############################################################################
  # The pattern for defining Klasses is the same as is used for defining
  # Commands. There is better documentation of this method there.
  #
  # TODO - consolidate the mechanics of the Command metaprogramming interface
  # with this one.
  class Klass

    ###########################################################################
    def setup(&block)
      @name = ""
      @exec = nil
      instance_eval &block if block_given?
    end

    ###########################################################################
    def get_name; @name; end
    def get_exec; @exec; end

    ###########################################################################
    def name(name)
      @name = name.to_s
    end

    def skill(skill, level, trains, difficulty, stamina, success_rate)

    end

    def dependency(independent, dependent)

    end

    def exec(&exec)
      @exec = exec
    end

  end

  #############################################################################
  def self.klass(klass_name, &init_block)
    class_name = "Klass_#{klass_name.to_s}"
    class_definition = Class.new(Klass) do
      define_method :initialize do
        method(:setup).call &init_block
        method(:name).call klass_name.to_s
      end
    end
    ::Talent.const_set class_name, class_definition
  end

end

