####################################################################################################
# Assess the environment.
####################################################################################################

# We assume development environment by default.
ENV['SYMPHONY_ENV'] ||= 'development'

# Standard practice puts the Gemfile at the project root.
ENV['GEMFILE'] ||= File.expand_path '../../Gemfile', __FILE__

####################################################################################################
# Consume the Gemfile and create our base App object.
#
# We break gem processing into two steps around App initialization so that App itself can make use
# of gems.
####################################################################################################

# Initialize gems from the Gemfile.
require 'bundler/setup' if File.exists? ENV['GEMFILE']

# Our application object. If you're familiar with Rails, this will feel familiar.
require File.expand_path '../app', __FILE__
Symphony = App.new

# Gorge ourselves on 3rd party gems...
Bundler.require :default, :setup, :app, Symphony.env

####################################################################################################
# Application autoload/autoconfig heavy lifting.
####################################################################################################

# Autoload application modules.
Dir[Symphony.root.join 'lib', '**', '*.rb'].each { |file| require file }

# Autoload application configuration scripts.
Dir[Symphony.root.join 'config', 'initializers', '*.rb'].each { |file| require file }
