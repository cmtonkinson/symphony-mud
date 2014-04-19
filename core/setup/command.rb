# Include all the command subclasses.
Dir[Symphony.root.join 'lib', 'commands', '*.rb'].each { |file| require file }

Command.configure do |config|
  config.command_sets[:global] = Command::CommandSet.factory [
    Command::Command_help.new,
    Command::Command_quit.new,
    Command::Command_shutdown.new,
    Command::Command_reboot.new,
  ]
end
