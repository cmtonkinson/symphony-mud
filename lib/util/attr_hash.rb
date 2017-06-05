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
    def transfer(obj, other_attr_hash)
      remove obj
      other_attr_hash.insert obj
    end

    ###########################################################################
    # Find Objekts by name in the AttrHash.
    # Returns an Array of Objekts on success.
    def locate(input)
      if input.is_a? String
        id_arg = Command::ARGUMENT_TYPES[:id]
        return false unless id_arg[:pattern].match input
        id = id_arg[:cast_func].call input
      elsif input.is_a? OpenStruct
        id = input
      end

      # Look for exact matches first.
      candidates = values.select { |obj| obj.name.downcase == id.name }

      # If no exact matches were found, look for prefix matches.
      candidates = values.select { |obj| obj.name.downcase.index(id.name) == 0 }

      # If no prefix matches were found, give up.
      return [] if candidates.empty? 

      case id.symbol
      when "#" # Return a single candidate (if exists) by 1-indexed list
        return [] if candidates.length < id.number
        [candidates[id.number - 1]]
      when "*" # Return some quantity of candidates
        how_many = case id.number
                   when Integer then id.number
                   when "all" then candidates.length
                   when "half" then candidates.length / 2
                   else 0
                   end
        candidates.take how_many
      else # Return the first candidate
        [candidates.first]
      end

    end

    ###########################################################################
    def display_contents(stack = true)
      values
        .group_by(&:name)
        .map { |name, objekts|
          pretty_name = Util::Linguist.interpret objekts[0].interpret_name, objekts[0]
          if stack
            if objekts.length > 1
              " {W[%d]{x %s" % [objekts.length, pretty_name]
            else
              " %s" % pretty_name
            end
          else
            " %s" % pretty_name
          end
        }
        .join("\n")
    end

  end

end

