####################################################################################################
# Bootstrap Symphony; this code is the test analogue of the main `symphony.rb` script.
####################################################################################################
BINARY_NAME = "" # stub
ENV['SYMPHONY_ENV'] ||= 'test'
require File.expand_path 'core/bootstrap'

####################################################################################################
# Override initializers, as necessary.
# TODO: Should these be broken out into a subfolder to more closely mirror config/initializers?
####################################################################################################
Network.configure do |config|
  config.listen_ip   = '127.0.0.1'
  config.listen_port = 6501
end

####################################################################################################
# Load our custom helpers.
####################################################################################################
Dir[Symphony.root.join 'spec', 'helpers', '*_helper.rb'].each { |file| require file }

####################################################################################################
# Configuration blocks for testing utilities.
####################################################################################################
RSpec.configure do |config|
  # config.treat_symbols_as_metadata_keys_with_true_values = true
  config.run_all_when_everything_filtered = true
  config.order = 'random'
end
