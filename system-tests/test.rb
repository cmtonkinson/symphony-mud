#!/usr/bin/env ruby

require 'pathname'
$ROOT_DIR = Pathname.new(__dir__).join('..')
$SYMPHONY = $ROOT_DIR.join('symphony.rb')
$TELNET_DELAY = 0.1

###############################################################################
puts "Bootstrapping system test suite..."

require_relative 'system_test/init'
Dir[File.join(__dir__, 'scenarios', '*.rb')].each { |file| require file }

m = SystemTest::Manager.new

###############################################################################
puts "Running system tests..."
m.start

###############################################################################
puts "System testing complete."

