require 'optparse'

###############################################################################
# Configuration configuration, with default configurations. It's turtles all
# the way down. Seriously though: this Array defines all of the settings
# available for the application. There is a strict order of precedence, from
# low to high:
#
#   1. Defaults (defined below)
#   2. Configuration file (e.g. /path/to/symphony/config.yml)
#   3. Environment variables (e.g. SYMPHONY_LOG_LEVEL)
#   4. Command line options (e.g. --log-level)
#
# Note that initializer scripts run BEFORE any of these are processed.
###############################################################################
CONFIGURATION = [
  # short: The single-character abbreviation to use on the command line
  # long: The long form command line name, used also for the environment variable
  # placeholder: For demonstrating required values, or nil
  # default: The value used in the absence of any overrides
  # description: Brief one-liner of what the option changes
  # callback: Proc that accepts the new value being passed in
  {
    short: 'b',
    long: 'bind',
    placeholder: 'ADDRESS',
    default: '127.0.0.1',
    description: 'Host address on which to listen',
    callback: Proc.new { |value| Network.configuration.listen_ip = value },
  },
  {
    short: 'c',
    long: 'console',
    default: false,
    description: 'Open an interactive console',
    callback: Proc.new { |value| Symphony.configuration.console = value },
  },
  {
    short: 'l',
    long: 'log-level',
    placeholder: 'LEVEL',
    default: 'info',
    description: 'Change the system logging verbosity',
    callback: Proc.new { |value| Symphony.logger.level = Logger.const_get(value.upcase) },
  },
  {
    short: 'p',
    long: 'port',
    placeholder: 'PORT',
    default: 6500,
    description: 'Server port on which to bind',
    callback: Proc.new { |value| Network.configuration.listen_port = value },
  },
  {
    short: 's',
    long: 'script',
    placeholder: 'FILE',
    default: false,
    description: 'Run a script',
    callback: Proc.new { |value| Symphony.configuration.script = value },
  },
]

###############################################################################
# Set all the defaults.
###############################################################################
CONFIGURATION.each { |config| config[:callback].call config[:default] }

###############################################################################
# Check for a configuration file.
###############################################################################
config_file = Symphony.root.join 'config.yml'

if File.readable? config_file
  config_hash = YAML.load File.read config_file
  CONFIGURATION.each do |config|
    yml_var_name = config[:long].tr '-', '_'
    if config_hash.key? yml_var_name
      config[:callback].call config_hash[yml_var_name]
    end
  end
end

###############################################################################
# Check the environment for configuration settings.
###############################################################################
CONFIGURATION.each do |config|
  # Convert the long name to an ENV_STYLE_TOKEN.
  env_var_name = "SYMPHONY_#{config[:long]}".upcase.tr '-', '_'
  # If the ENV var is found, invoke the callback with its value.
  if ENV.key? env_var_name
    config[:callback].call ENV[env_var_name]
  end
end

###############################################################################
# Check the command line for configuration settings.
###############################################################################
# We want to skip over the OptionParser if the application is being loaded in
# the context of another program (e.g. rspec) or this will cause any CLI args
# intended for that program to be processd here.
$command_line_args = ARGF.argv.map { |a| ' ' + a }.reduce(:+) || ''
if $0 =~ /symphony/
  OptionParser.new do |opts|
    opts.banner = "Usage: #{BINARY_NAME} [options]"

    CONFIGURATION.each do |config|
      short = '-' + config[ :short]
      long  = '--' + config[:long] + (config.key?(:placeholder) ? " #{config[:placeholder]}" : '')
      desc  = config[:description]
      func  = config[:callback]
      opts.on short, long, desc, func
    end

  end.parse!
end
