module Base

  class Terminal

    RESET   = "\e[0m"
    BOLD    = "\e[1m"

    COLORS = {
      "gray":           {code: "w", escape: "\e[30m#{BOLD}"},
      "white":          {code: "W", escape: "\e[37m#{BOLD}"},
      "red":            {code: "r", escape: "\e[31m"},
      "bright red":     {code: "R", escape: "\e[31m#{BOLD}"},
      "green":          {code: "g", escape: "\e[32m"},
      "bright green":   {code: "G", escape: "\e[32m#{BOLD}"},
      "yellow":         {code: "y", escape: "\e[33m"},
      "bright yellow":  {code: "Y", escape: "\e[33m#{BOLD}"},
      "blue":           {code: "b", escape: "\e[34m"},
      "bright blue":    {code: "B", escape: "\e[34m#{BOLD}"},
      "magenta":        {code: "m", escape: "\e[35m"},
      "bright magenta": {code: "M", escape: "\e[35m#{BOLD}"},
      "cyan":           {code: "c", escape: "\e[36m"},
      "bright cyan":    {code: "C", escape: "\e[36m#{BOLD}"},
    }

    def self.escape_map
      @escape_map ||= {
        "{x" => RESET,
        "{{" => "{",
      }.merge(Hash[COLORS.map do |k,v|
        ["{#{v[:code]}", v[:escape]]
      end])
    end

    def self.colorize(source)
      source.gsub /\{[WwrRgGyYbBmMcCx{]/, escape_map
    end

  end

end
