####################################################################################################
# Slurp up all Command definitions.
####################################################################################################
Dir[Symphony.root.join 'lib', 'commands', '*.rb'].each { |file| require file }

####################################################################################################
# Command post-processing. First we've got to get a list of unique groups, then we add each command
# (in the order in which they were defined) to its appropriate group.
####################################################################################################
Command.configure do |config|
  # Create a CommandSet for each needed group.
  config.command_registry.map(&:get_group).uniq.sort.each do |group|
    config.command_sets[group] = Command::CommandSet.new
  end
  # Add each command to the appropriate group.
  config.command_registry.each do |command|
    config.command_sets[command.get_group] << command
  end
end
