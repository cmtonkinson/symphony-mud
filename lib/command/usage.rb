module Command

  class Usage
    attr_reader :data

    ###########################################################################
    # Create the underlying storage.
    def initialize
      @data = []
    end

    ###########################################################################
    # Add a new Argument. Raise if something goes wrong.
    def push(arg)
      # Don't accept custom data structures.
      unless arg.is_a? Argument
        raise ArgumentError.new I18n.t "command.usage.push_non_argument"
      end
      # Argument names must be unique.
      if @data.any? { |a| a.name == arg.name }
        raise ArgumentError.new I18n.t "command.usage.duplicate_name", name: arg.name
      end
      # Append.
      @data << arg
    end
    alias_method :<<, :push

    ###########################################################################
    def try_parse(input)
      remaining = input.dup
      context   = OpenStruct.new

      each do |argument|
        substr = argument.scan remaining
        if substr == false
          return nil
        end

        context[argument.name] = argument.cast substr
        remaining.slice! 0, substr.length
        remaining.strip!
      end

      if !remaining.match /\A\s*\z/
        return nil
      end

      return context
    end

    ###########################################################################
    # This makes sure that Usages are sorted by the number of Arguments they
    # contain, descending. The exception is that a Usage defined with only a
    # :rest and nothing else should always appear at the end of the list so as
    # not to overshadow other invocations with more specific Arguments.
    def sort_method
      num_args = @data.length
      if num_args == 1 && @data.first.type == :rest
        return 999
      else
        return -num_args
      end
    end

    ###########################################################################
    # Proxy all other methods down to the @data Array.
    def method_missing(meth, *args, &block)
      @data.send meth, *args, &block
    end

    ###########################################################################
    def to_s
      dest = "Usage<"

      dest += "#{length}:"
      dest += map(&:to_s).join ","

      dest += ">"
      dest
    end

  end
end
