module Command

  class Usage < Array
    ###########################################################################
    # Add a new Argument. Raise if something goes wrong.
    def push(arg)

      # Don't accept custom data structures.
      unless arg.is_a? Argument
        raise ArgumentError.new I18n.t 'command.usage.push_non_argument'
      end

      # Argument names must be unique.
      if any? { |a| a.name == arg.name }
        raise ArgumentError.new I18n.t 'command.usage.duplicate_name', name: arg.name
      end

      # Append.
      super
    end

    ###########################################################################
    def try_parse(input)
      remaining = input.dup
      context   = OpenStruct.new

      each do |argument|
        substr = argument.scan remaining
        return nil if substr == false

        return nil if argument.type == :literal && substr != argument.name

        context[argument.name] = argument.cast substr
        remaining.slice! 0, substr.length
        remaining.strip!
      end

      return nil if !remaining.match /\A\s*\z/

      context
    end

    ###########################################################################
    # This makes sure that Usages are sorted by the number of Arguments they
    # contain, descending. The exception is that a Usage defined with only a
    # :rest and nothing else should always appear at the end of the list so as
    # not to overshadow other invocations with more specific Arguments.
    def sort_method
      length == 1 && first.type == :rest ? 999 : -length
    end

    ###########################################################################
    def to_s
      "Usage<#{length}:#{map(&:to_s).join(',')}>"
    end
  end
end
