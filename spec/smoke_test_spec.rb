require 'spec_helper'

describe "smoke test" do

  describe "TCPServer" do

    before :each do
      @world = Core::World.new
    end

    it "should accept new connections" do
      expect { TCPSocket.new Network.configuration.listen_ip, Network.configuration.listen_port }.not_to raise_error
    end

  end

end
