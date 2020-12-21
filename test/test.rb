#!/usr/bin/env ruby

test_start = Time.now

require_relative 'configure'

###############################################################################
puts 'Bootstrapping system test suite...'
puts "Seed #{$RAND_SEED}"

require_relative 'test/init'
Dir[File.join(__dir__, 'scenarios', '*.rb')].sort.each { |file| require file }

m = Test::Manager.new

print "Loading complete in %.3fs\n\n" % (Time.now - test_start).to_f

###############################################################################
m.start

###############################################################################
print "\n\nSeed %s\nSystem testing complete in %.3fs.\n" % [$RAND_SEED, (Time.now - test_start).to_f]
puts "  #{m.num_tests} tests".colorize(:yellow)
puts "  #{m.num_pass} passed".colorize(:green)
puts "  #{m.num_fail} failed".colorize(:red)

exit m.num_fail
