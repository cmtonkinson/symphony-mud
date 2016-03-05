module Util

  class AttrHash
    attr_reader :attribute

    def initialize(attribute_name = :id)
      @attribute = attribute_name.to_sym
      @hash = {}
    end

    def insert(obj)
      @hash.store obj.send(@attribute), obj
    end
    alias_method :<<, :insert

  end

end
