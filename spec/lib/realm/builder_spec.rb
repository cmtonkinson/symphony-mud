require 'spec_helper'

include Realm

describe Realm::Builder do

  #############################################################################
  describe "#create_room" do

    before :each do
      @zone = Zone.new
      @room = Builder.create_room @zone
    end

    it "returns a new Room" do
      expect(@room).to be_a Room
    end

    it "connects the Room to the Zone" do
      expect(@room.zone).to eq @zone
    end

    it "registers the Room with the Zone" do
      expect(@zone.rooms.keys).to include @room.id
    end

    it "assign a rid to the Room" do
      expect(@room.rid).to be_a Integer
    end

  end

  #############################################################################
  describe "#link_existing_rooms" do

    before :each do
      @zone   = Zone.new
      @room_a = Builder.create_room @zone
      @room_b = Builder.create_room @zone

      Builder.link_existing_rooms @room_a, @room_b, "east"
    end

    it "sets both exits in opposing directions" do
      expect(@room_a.east_room).to be @room_b
      expect(@room_b.west_room).to be @room_a
    end

  end

  #############################################################################
  describe "#create_linked_room" do

    before :each do
      @zone   = Zone.new
      @room_a = Builder.create_room @zone
      @room_b = Builder.create_linked_room @room_a, "east"
    end

    it "returns a new Room" do
      expect(@room_b).to be_a Room
    end

    it "adds the new Room to the Zone" do
      expect(@zone.rooms.keys).to include @room_b.id
      expect(@room_b.zone).to eq @zone
    end

    it "sets the coordinates of the new Room" do
      expect(@room_b.coords).to eq Vector[1, 0, 0]
    end

  end

end
