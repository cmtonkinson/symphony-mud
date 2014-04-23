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
  attr_reader :env, :binary, :root, :modules, :setup, :initializers

  ENGINE_DIRNAME       = 'engine'
  CONFIG_DIRNAME       = 'config'
  MODULE_DIRNAME       = 'modules'
  SETUP_DIRNAME        = 'setup'
  INITIALIZERS_DIRNAME = 'initializers'

  def initialize
    @env     = ENV['SYMPHONY_ENV'].to_sym
    @root    = Pathname.new File.expand_path '../..', __FILE__
    @binary  = @root.join BINARY_NAME
    @modules = @root.join ENGINE_DIRNAME, MODULE_DIRNAME
    @setup   = @root.join ENGINE_DIRNAME, SETUP_DIRNAME
    @initializers = @root.join CONFIG_DIRNAME, INITIALIZERS_DIRNAME
  end

end
