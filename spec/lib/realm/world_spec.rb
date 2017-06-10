require 'spec_helper'

include Realm

describe Realm::World do

  describe "#next_room_rid" do

    before :each do
      @z = Builder.create_zone
    end

    it "should return an Integer" do
      expect($world.next_room_rid).to be_a Integer
    end

  end

end
