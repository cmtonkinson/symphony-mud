require 'spec_helper'

include World

describe World::Zone do

  describe "#" do

    it "should work" do
      z = Zone.new
      expect(true).to eq true
    end

  end

end
