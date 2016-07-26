###############################################################################
# If you're familiar with Rails, a lot of the patterns for this App class will
# be familiar to you.
#
# @root is a Pathname to the root project directory.
# @env is a symbol representing the current environment, and is typically one of
#   - :development (default)
#   - :test
#   - :production
#
# This class is designed to be a globally-available singleton.
###############################################################################
class App
  DIRECTORIES = {
    config: %i(initializers locales setup),
    data: %i(),
    game: %i(commands templates),
    lib: %i(),
  }

  # Create a reader accessor for each project directory.
  DIRECTORIES.each do |dir, subdirs|
    attr_reader dir
    subdirs.each do |subdir|
      attr_reader subdir
    end
  end

  # Other readers and accessors.
  attr_reader :env, :root, :accounts
  attr_accessor :logger

  def initialize
    @env    = ENV["SYMPHONY_ENV"].to_sym
    @root   = Pathname.new File.expand_path "../..", __FILE__
    @logger = nil

    DIRECTORIES.each do |dir, subdirs|
      instance_variable_set "@#{dir}", @root.join(dir.to_s)
      subdirs.each do |subdir|
        instance_variable_set "@#{subdir}", @root.join(dir.to_s, subdir.to_s)
      end
    end
  end

end
