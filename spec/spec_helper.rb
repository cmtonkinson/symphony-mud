# Bootstrap Symphony
ENV['SYMPHONY_ENV'] ||= 'test'
require File.expand_path '_chassis/bootstrap'

# Override module configuration where necessary.
Network.configure do |config|
  config.listen_ip   = '127.0.0.1'
  config.listen_port = 6501
end

# Configure RSpec
RSpec.configure do |config|
  config.treat_symbols_as_metadata_keys_with_true_values = true
  config.run_all_when_everything_filtered = true
  config.order = 'random'
end
