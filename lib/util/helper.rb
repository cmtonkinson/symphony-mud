module Util
  class Helper
    def self.print_usages_for(command)
      usages = command.usages.reverse

      usages.map do |usage|
        output = '  %s %s' % [command.get_name, usage.map(&:help_name).join(' ')]
        usage.each do |arg|
          next if arg.type == :literal

          output << "\n    where %s is %s" % [arg.help_name, arg.help_description]
        end
        output
      end.join "\n"
    end
  end
end
