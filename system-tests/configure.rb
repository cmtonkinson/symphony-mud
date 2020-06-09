require 'pathname'
require 'optparse'

$ROOT_DIR = Pathname.new(__dir__).join('..')
$SYMPHONY = $ROOT_DIR.join('symphony.rb')
$TELNET_DELAY = 0.05

$RAND_SEED = Random.new_seed.to_s[0, 8]

OptionParser.new do |opts|
  opts.on('-l,', '--leaf TEST', 'Only execute TEST') do |value|
    $LEAF_TEST = value
  end

  opts.on('-s', '--seed SEED', 'Set the PRNG seed') do |value|
    $RAND_SEED = value
  end

  opts.on('-t', '--tree TEST', 'Execute TEST and all its descendents') do |value|
    $TREE_TEST = value
  end
end.parse!

if $LEAF_TEST && $TREE_TEST
  puts '--leaf and -tree are mutually exclusive.'
  exit 1
end

srand $RAND_SEED.to_i
