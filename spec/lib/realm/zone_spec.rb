require 'spec_helper'

include Realm

describe Realm::Zone do

  describe "#" do

    it "should work" do
      z = Zone.new
      expect(true).to eq true
    end

  end

end
