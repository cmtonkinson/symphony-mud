require 'pathname'
require 'optparse'

$ROOT_DIR = Pathname.new(__dir__).join('..')
$SYMPHONY = $ROOT_DIR.join('symphony.rb')
$TELNET_DELAY = 0.1

$RAND_SEED = Random.new_seed.to_s[0,8]

OptionParser.new { |opts|
  opts.on('-s', '--seed SEED', 'Set the PRNG seed') do |value|
    puts "GOT #{value.inspect}"
    $RAND_SEED = value
  end
}.parse!

srand $RAND_SEED.to_i


