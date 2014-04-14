Core.configure do |config|
  config.sleep_interval = 0.01
end

Network.configure do |config|
  config.listen_ip   = '0.0.0.0'
  config.listen_port = 6500
end
