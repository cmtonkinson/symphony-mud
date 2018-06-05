module Command
  ARGUMENT_TYPES = {
    int: {
      pattern: / [+-]? \d+ /ix,
      cast_func: :to_i,
      description: "an integer (or 'whole number'); a number with no decimal point",
      examples: [-1, 0, 1, 42, +1],
    },
    decimal: {
      pattern: / [+-]? \d+\.\d+ /ix,
      cast_func: :to_f,
      description: 'a number with a decimal point',
      examples: [3.14, 1.618, +4.2, 0.0, -5.0],
    },
    bool: {
      pattern: / t(?:rue)? | f(?:alse)? | y(?:es)? | n(?:o)? | 1 | 0 /ix,
      cast_func: -> (b, regexp) {
        return !/t(?:rue)|y(?:es)|1/.match(b).nil?
      },
      description: 'a true or false value',
      examples: ['t', 'true', 'f', 'false', 'y', 'yes', 'n', 'no', 0, 1],
    },
    token: {
      pattern: / [\w\d]+ /ix,
      description: 'a sequence of letters, digits, and underscores - no spaces or punctuation',
      examples: ['batman', 'off_set', 'field1'],
    },
    literal: {
      pattern: / \S+ /ix,
      description: -> arg { "the literal text \"#{arg.name}\"" },
      examples: ['in', 'to', 'at'],
    },
    set: {
      pattern: nil,
      description: -> arg { 'any one of: ' + arg.value.join(', ') },
      examples: ['silver,gold', 'to,from,on,at'],
    },
    string: {
      pattern: / [^'"\s]+ | (?:('|") [^\1]*? \1) /ix,
      cast_func: -> (src) {
        c = src[0]
        return (c == '"' || c == "'") ? src.slice(1..-2) : src
      },
      description: 'arbitrary text (can include letters, numbers, words, spaces, and puctuation)',
      examples: ['hello world', 'you won 2nd place!', "can you say 'hello world?'"],
    },
    id: {
      pattern: / (?:[^'"\s]+ | (?:('|") [^\1]+? \1)) (?:([#*]) (\d+|all|half))? /ix,
      cast_func: -> (x) {
        name, symbol, number = x.partition /#|\*/
        id = OpenStruct.new(
          name: name.downcase.sub(/('|") (.+?) \1/ix, '\2'),
          symbol: symbol,
          number: if %w[all half].include?(number) then number else number.to_i end
        )
      },
      description: 'a target object name (with optional quotes and optional multiplier or offset)',
      examples: ['elf', 'rat#2', 'bread*5', "'short sword'"],
    },
    rest: {
      pattern: /\A.+/ix,
      cast_func: -> (src) {
        c = src[0]
        return (c == '"' || c == "'") ? src.slice(1..-2) : src
      },
      description: 'any other text on the line (e.g. the remainder of the input)',
    },
  }


  # This shitty indirection is just so that we can automate assertions that no one is creating
  # Command formats with variable names (which become context methods) that would overlap default
  # context methods in the `exec` block. See Command#perform as well as Command#usage for more.
  DEFAULT_CONTEXT = %i[user full_input being room]

  class Argument
    attr_reader :name, :type, :value

    ###########################################################################
    def initialize(definition = nil)
      @empty = true
      study definition
    end

    ###########################################################################
    def empty?
      @empty
    end

    ###########################################################################
    def study(definition)
      unless definition.nil?
        @empty = false

        case definition
        when Hash
          study_hash *definition.first
        when Symbol
          if definition == :rest
            @type = :rest
            @name = 'rest'
          else
            @type = :id
            @name = definition.to_s
          end
        when String
          @type  = :literal
          @name  = definition
        else
          raise ArgumentError.new I18n.t('command.argument.bad_argument_definition', arg: definition.inspect)
        end

        if DEFAULT_CONTEXT.include? @name
          raise ArgumentError.new I18n.t('command.variable_overshadow', name: @name)
        end
      end
    end

    ###########################################################################
    def study_hash(name, kind)
      @name = name.to_s
      if kind.is_a? Array
        @type = :set
        @value = []
        kind.each do |elt|
          if !elt.is_a? String
            raise ArgumentError.new I18n.t 'command.argument.bad_array_value', type: elt.class
          end
          @value << elt
        end
      else
        @type = kind.to_sym
      end
    end

    ###########################################################################
    def scan(input)
      case @type
      when :set then scan_for_set input
      else scan_for_other input
      end
    end

    ###########################################################################
    def scan_for_set(input)
      next_token = input.partition(' ').first
      if @value.include? next_token
        return next_token
      else
        return false
      end
    end

    ###########################################################################
    def scan_for_other(input)
      if @type == :rest
        pattern = /\A.+/ix
      else
        pattern = ARGUMENT_TYPES[@type][:pattern]
      end

      source = pattern.source
      lexer  = /\A(?:#{source})\b?/ix
      match  = lexer.match input

      # The beginning of input did not match the RegExp for the expected
      # Argument type. That means the input string does not match this Usage
      # Usage and we should move on.
      if match.nil?
        return false
      end

      # Capture the input corresponding to this Argument.
      return match[0]
    end

    ###########################################################################
    def cast(src)
      # Use the String casting method when the Argument type is :rest
      if type == :rest
        return ARGUMENT_TYPES[:string][:cast_func].call src
      end

      # Fail if the Argument type isn't defined.
      return src unless ARGUMENT_TYPES.include? @type

      # Retrieve the casting instruction.
      cast_func = ARGUMENT_TYPES[@type][:cast_func]

      # Execute the casting operation and return the result.
      case cast_func
      when nil    then src
      when Symbol then src.send cast_func
      when Proc   then cast_func.call src
      end
    end

    ###########################################################################
    def to_s
      dest = 'Argument<'

      if empty?
        dest << 'empty'
      else
        dest << "#{name}(#{type}"
        dest << ":#{value}" if !value.nil?
        dest << ')'
      end

      dest << '>'
      dest
    end

    ###########################################################################
    def help_name
      case @type
      when :literal then @name
      when :set then @name
      when :rest then '...'
      else @name.upcase
      end
    end

    ###########################################################################
    def help_description
      defn = ARGUMENT_TYPES[@type][:description]
      case defn
      when String then defn
      when Proc then defn.call self
      when Array then defn.join '|'
      else raise "No handler for description definition of type #{defn.class.name} for Argument type '#{@type}'"
      end
    end
  end
end
