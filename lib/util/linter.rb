require 'open3'

module Util

  class Linter
    ###########################################################################
    # This method can search the entire project source code for any syntax
    # errors as would be reported by the Ruby interpreter. It's used as a pre-
    # flight check when rebooting the system.
    def self.lint_project_source
      # RbConfig will give us the path to the current ruby interpreter binary,
      # and should be accurate no matter how Ruby is installed or managed.
      ruby_bin = RbConfig::CONFIG['bindir'] + '/' + RbConfig::CONFIG['ruby_install_name']

      # Pipe the content of all .rb files in the project directory into Ruby's
      # built-in syntax checking function.
      lint_command = "find #{Symphony.root} -name '*.rb' | xargs cat | #{ruby_bin} -c > /dev/null"

      # The exit status of `ruby -c` will tell us whether there were errors,
      # and if there are errors the stderr of that process will hold the
      # diagnostic output (e.g. what the problem(s) are).
      lint_stdout, lint_stderr, lint_status = Open3.capture3 lint_command

      # Raise a SyntaxError if a syntax error is found.
      raise SyntaxError.new lint_stderr unless lint_status == 0
    end
  end

end
