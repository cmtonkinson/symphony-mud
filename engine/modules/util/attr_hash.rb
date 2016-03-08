module Util

  class AttrHash < Hash
    attr_reader :attribute

    def initialize(attribute_name = :id)
      @attribute = attribute_name.to_sym
    end

    def insert(obj)
      raise ArgumentError.new "#{obj} does not respond to :#{@attribute}" unless obj.respond_to? @attribute
      store obj.send(@attribute), obj
    end
    alias_method :<<, :insert

  end

end
