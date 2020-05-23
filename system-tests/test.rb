#!/usr/bin/env ruby

require 'pathname'
$ROOT_DIR = Pathname.new(__dir__).join('..')
$SYMPHONY = $ROOT_DIR.join('symphony.rb')
$TELNET_DELAY = 0.1

###############################################################################
puts "Bootstrapping system test suite..."
puts "Seed #{Random.new.seed}"

require_relative 'system_test/init'
Dir[File.join(__dir__, 'scenarios', '*.rb')].each { |file| require file }

m = SystemTest::Manager.new

###############################################################################
puts "\n"
m.start

###############################################################################
puts "\n\nSystem testing completed."
puts "  #{m.num_tests} tests".colorize(:yellow)
puts "  #{m.num_pass} passed".colorize(:green)
puts "  #{m.num_fail} failed".colorize(:red)

exit m.num_fail

