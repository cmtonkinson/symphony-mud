module Util
  class AttrHash < Hash
    include Util::Locatable

    attr_reader :attribute

    ###########################################################################
    def initialize(attribute_name = :id)
      @attribute = attribute_name.to_sym
    end

    ###########################################################################
    def insert(obj)
      store attribute_value(obj), obj
    end

    ###########################################################################
    def delete(obj)
      super attribute_value(obj)
    end

    ###########################################################################
    def attribute_value(obj)
      raise ArgumentError.new "#{obj} does not respond to :#{@attribute}" unless obj.respond_to? @attribute

      obj.send @attribute
    end

    ###########################################################################
    def transfer(obj, other_attr_hash)
      delete obj
      other_attr_hash.insert obj
    end

    ###########################################################################
    def display_contents(stack = true)
      values
        .group_by(&:name)
        .map do |_name, objekts|
          pretty_name = Util::Linguist.interpret objekts[0].interpret_name, objekts[0]
          if stack
            if objekts.length > 1
              ' {W[%d]{x %s' % [objekts.length, pretty_name]
            else
              ' %s' % pretty_name
            end
          else
            ' %s' % pretty_name
          end
        end
        .join("\n")
    end
  end
end
