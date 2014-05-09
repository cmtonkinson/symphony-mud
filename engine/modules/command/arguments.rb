module Command

  ARGUMENT_TYPES = {
    int: {
      pattern:      /\A[+-]?\d+\Z/,
      description:  "an integer; a number with no decimal point",
      examples:     [-1, 0, 1, 42],
    },
    float: {
      pattern:      /\A[+-]?\d+\.\d+\Z/,
      description:  "a number with a decimal point",
      examples:     [3.14, 1.618, 0.0, -5.0],
    },
    bool: {
      pattern:      /\At(?:rue)?|f(?:alse)?|1|0\Z/i,
      description:  "a true or false value",
      examples:     ["t", "true", "f", "false", 0, 1],
    },
    letter: {
      pattern:      /\A[a-z]\Z/i,
      description:  "a single letter",
      examples:     ["a", "Z"],
    },
    word: {
      pattern:      /\A[a-z]+\Z/i,
      description:  "a single word (no digits, no spaces)",
      examples:     ["apple", "sword", "glass"],
    },
    token: {
      pattern:      /\A\w+\Z/i,
      description:  "a sequence of letters, digits, and underscores - no spaces or punctuation",
      examples:     ["batman", "off_set", "field1"],
    },
    # string: {
    #   pattern:      /\A('|")[^'"]*(\1)\Z/i,
    #   description:  "arbitrary text (can include letters, numbers, words, spaces, and puctuation)",
    #   examples:     ["hello world", "you won 2nd place", "can haz halp?"],
    # },
    id: {
      pattern:      /\A('|")([\w\s]+)(\1)(?:([#*])(\d))?\Z/i,
      description:  "a target name, with optional quotes, multiplier, or offset",
      examples:     ["sword", "rat#2", "bread*5", "'broad sword'"],
    }
  }

  # This shitty indirection is just so that we can automate assertions that no one is creating
  # Command formats with names overlapping auto-variables in the `exec` block. See Command#perform
  # as well as Command#format for context.
  STATE_KEYS = {
    :user         => :user,
    :input_string => :input_string,
  }

end
