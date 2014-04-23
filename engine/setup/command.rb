# Initialize our CommandSets.
Command.configure do |config|
  config.command_sets[:global] = Command::CommandSet.new
end

# Include all the command subclasses.
Dir[Symphony.root.join 'lib', 'commands', '*.rb'].each { |file| require file }
