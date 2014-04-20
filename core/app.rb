####################################################################################################
# If you're familiar with Rails, a lot of the patterns for this App class will be familiar to you.
# @root is a Pathname to the root project directory.
# @env is a symbol representing the current environment, and is typically one of
#   - :development (default)
#   - :test
#   - :production
#
# This class is designed to be a globally-available singleton.
####################################################################################################
class App
  attr_reader :binary, :root, :env

  def initialize
    @root   = Pathname.new File.expand_path '../..', __FILE__
    @binary = @root.join BINARY_NAME
    @env    = ENV['SYMPHONY_ENV'].to_sym
  end

end
