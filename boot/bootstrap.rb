# Load gems and create the App object.
# Standard practice puts the Gemfile at the project root.
ENV['GEMFILE'] ||= File.expand_path '../Gemfile', __dir__
require 'bundler/setup'
Bundler.require :default

# Our application object, designed to be a global singleton.
Symphony = App.new

# I18n needs to come in before we start autoloading.
I18n.enforce_available_locales = true
I18n.default_locale = :en

I18n.load_path += Dir[Symphony.root.join 'locales', '**', '*.yml']
I18n.load_path += Dir[Symphony.game.join 'locales', '**', '*.yml']

# Load the Core engine and game components.
# Definitions first.
require Symphony.root.join 'lib', 'konst.rb'

# Are there game-level definition overrides?
game_konst = Symphony.root.join 'game', 'konst.rb'
require game_konst if File.file? game_konst

# Autoload core modules.
Dir[Symphony.root.join 'lib', '*']
  .reject { |file| File.file? file }
  .sort
  .each { |mod| require "#{mod}/_init" }

# Look for a custom loader at game/load.rb, otherwise autoload all of game/lib
game_loader = Symphony.root.join 'game', 'load'
if File.file? game_loader
  require game_loader
else
  Dir[Symphony.root.join 'game', 'lib', '**', '*.rb'].each { |file| require file }
end

# Load command definitions.
Dir[Symphony.root.join 'commands', '**', '*.rb'].each { |file| require file }

Dir[Symphony.root.join 'game', 'commands', '**', '*.rb'].each { |file| require file }

# Configure the application.
Dir[Symphony.root.join 'boot', 'setup', '*.rb'].each { |file| require file }

Dir[Symphony.root.join 'game', 'setup', '*.rb'].each { |file| require file }

# Other external configuration.
require_relative 'configuration'

# Big Bang.
$universe = Base::Universe.new
$world    = Realm::World.new

trap('USR1') do
  $universe.should_save = true
  $universe.should_stop = true
end

# Write the pid to a file.
IO.write(Symphony.pidfile, Process.pid.to_s, mode: 'w')
