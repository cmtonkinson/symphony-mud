# Assess the environment.
ENV['SYMPHONY_ENV'] ||= 'development'
ENV['GEMFILE'] ||= File.expand_path '../../Gemfile', __FILE__

# Initialize gems from the Gemfile.
require 'bundler/setup' if File.exists? ENV['GEMFILE']

# Our application object.
require File.expand_path '../engine', __FILE__
Symphony = Engine.new

# Gorge ourselves on 3rd party gems...
Bundler.require :default, :setup, :app, Symphony.env

# Load up our library modules.
Dir[Symphony.root.join 'lib', '**', '*.rb'].each { |file| require file }

# Pass control to custom module configuration blocks.
require File.expand_path 'config/setup'
