require 'spec_helper'

include Realm

describe Realm::World do

  describe "#next_room_rid" do

    before :each do
      $world = World.new
      @z = Zone.new
      $world.insert @z
    end

    it "should return 1 by default" do
      expect($world.next_room_rid).to eq 1
    end

    it "return one more than the highest Room rid" do
      @z.insert Room.new(nil, rid: 1)
      @z.insert Room.new(nil, rid: 2)
      @z.insert Room.new(nil, rid: 3)
      expect($world.next_room_rid).to eq 4
    end

    it "skip gaps" do
      @z.insert Room.new(nil, rid: 4)
      @z.insert Room.new(nil, rid: 20)
      @z.insert Room.new(nil, rid: 37)
      expect($world.next_room_rid).to eq 38
    end

  end

end
