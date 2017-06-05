module Util

  class AttrHash < Hash
    attr_reader :attribute

    ###########################################################################
    def initialize(attribute_name = :id)
      @attribute = attribute_name.to_sym
    end

    ###########################################################################
    def insert(obj)
      store attribute_value(obj), obj
    end
    alias_method :<<, :insert

    ###########################################################################
    def remove(obj)
      delete attribute_value(obj)
    end
    alias_method :-, :remove

    ###########################################################################
    def attribute_value(obj)
      raise ArgumentError.new "#{obj} does not respond to :#{@attribute}" unless obj.respond_to? @attribute
      obj.send @attribute
    end

    ###########################################################################
    def locate(input)
      ids = input.match Command::ARGUMENT_TYPES[:id][:pattern]
      puts Command::ARGUMENT_TYPES[:id][:pattern].inspect
      puts "input: " + input.inspect
      puts "ids: " + ids.inspect

      f = Command::ARGUMENT_TYPES[:id][:cast_func]

      i = ids.map { |id| f.call id[0] }
        .map { |obj| obj.name }
        .reduce :+

      puts "realsies: #{i}"
    end

  end

end
