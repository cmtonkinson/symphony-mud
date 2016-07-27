require "optparse"

###############################################################################
# Configuration configuration, with default configurations. Yo dawg.
# Seriously though: this Array defines all of the settings available for the
# application. There is a strict order of precedence, from low to high:
#
#   1. Defaults (defined below)
#   2. Environment variables (e.g. SYMPHONY_LOG_LEVEL)
#   3. Command line options (e.g. --log-level)
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
    short: "c",
    long: "console",
    default: false,
    description: "Open an interactive console",
    callback: Proc.new { |value| Symphony.configuration.console = value }
  },
  {
    short: "l",
    long: "log-level",
    placeholder: "LEVEL",
    default: "info",
    description: "Change the system logging verbosity",
    callback: Proc.new { |value| Logger.configuration.verbosity = value.to_sym }
  },
]

###############################################################################
# Set all the defaults.
###############################################################################
CONFIGURATION.each { |config| config[:callback].call config[:default] }

###############################################################################
# Check the environment for configuration settings.
###############################################################################
CONFIGURATION.each do |config|
  # Convert the long name to an ENV_STYLE_TOKEN.
  env_var_name = "SYMPHONY_#{config[:long]}".upcase.gsub /-/, "_"
  # If the ENV var is found, invoke the callback with its value.
  if ENV.key? env_var_name
    config[:callback].call ENV[env_var_name]
  end
end

###############################################################################
# Check the command line for configuration settings.
###############################################################################
OptionParser.new do |opts|
  opts.banner = "Usage: symphony.rb [options]"

  CONFIGURATION.each do |config|
    short = "-" + config[ :short]
    long  = "--" + config[:long] + (config.key?(:placeholder) ? " #{config[:placeholder]}" : "")
    desc  = config[:description]
    func  = config[:callback]
    opts.on short, long, desc, func
  end

end.parse!
