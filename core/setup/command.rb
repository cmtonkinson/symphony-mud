# Include all the command subclasses.
Dir[Symphony.root.join 'lib', 'commands', '*.rb'].each { |file| require file }

Command.configure do |config|
  config.command_sets << Command::CommandSet.factory("global", [
    Command::Command_help.new,
  ])
end
