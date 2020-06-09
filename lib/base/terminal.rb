module Base
  class Terminal
    RESET   = "\e[0m".freeze
    BOLD    = "\e[1m".freeze

    COLORS = {
      "gray": { code: 'w', escape: "\e[30m#{BOLD}" },
      "white": { code: 'W', escape: "\e[37m#{BOLD}" },
      "red": { code: 'r', escape: "\e[31m" },
      "bright red": { code: 'R', escape: "\e[31m#{BOLD}" },
      "green": { code: 'g', escape: "\e[32m" },
      "bright green": { code: 'G', escape: "\e[32m#{BOLD}" },
      "yellow": { code: 'y', escape: "\e[33m" },
      "bright yellow": { code: 'Y', escape: "\e[33m#{BOLD}" },
      "blue": { code: 'b', escape: "\e[34m" },
      "bright blue": { code: 'B', escape: "\e[34m#{BOLD}" },
      "magenta": { code: 'm', escape: "\e[35m" },
      "bright magenta": { code: 'M', escape: "\e[35m#{BOLD}" },
      "cyan": { code: 'c', escape: "\e[36m" },
      "bright cyan": { code: 'C', escape: "\e[36m#{BOLD}" },

      "reset": { code: 'x', escape: RESET },
      "bracket": { code: '{', escape: '{' }
    }.freeze

    # The negative lookbehind for % is so that this class' behavior doesn't
    # interefere with I18n interpolation syntax (which is `%{varname}`)
    # otherwise we could end up accidentally sending a string like `%arname}`
    # to `I18n#t`.
    VALID_CODES    = COLORS.values.map { |defn| defn[:code] }
    SPECIAL_CODES  = [RESET, '{', '?'].freeze
    CODE_PATTERN   = /(?<!%)\{[#{VALID_CODES.join}]/.freeze
    ESCAPE_PATTERN = /\e\[3\dm|\e\[\dm/.freeze

    def self.escape_map
      @escape_map ||= Hash[COLORS.map do |_k, v|
        ["{#{v[:code]}", v[:escape]]
      end]
    end

    def self.colorize(source)
      source.gsub!(/\{\?/) do |_|
        COLORS.values.map { |v| v[:escape] }
              .reject { |x| SPECIAL_CODES.include?(x) }
              .sample
      end
      source.gsub CODE_PATTERN, escape_map
    end

    def self.uncolor(source)
      source.gsub CODE_PATTERN, ''
    end

    def self.unescape(source)
      source.gsub ESCAPE_PATTERN, ''
    end
  end
end
