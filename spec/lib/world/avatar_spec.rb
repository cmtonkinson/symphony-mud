require 'spec_helper'

include World

describe World::Avatar do

  describe "#" do

    it "should work" do
      a = Avatar.new
      expect(true).to eq true
    end

  end

end
