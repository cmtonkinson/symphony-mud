require 'spec_helper'

describe Logistics::Builder do
  #############################################################################
  describe '#create_room' do
    before :each do
      @zone = Realm::Zone.new
      @room = Logistics::Builder.create_room @zone
    end

    it 'returns a new Room' do
      expect(@room).to be_a Realm::Room
    end

    it 'connects the Room to the Zone' do
      expect(@room.zone).to eq @zone
    end

    it 'registers the Room with the Zone' do
      expect(@zone.rooms.keys).to include @room.id
    end

    it 'assign a rid to the Room' do
      expect(@room.rid).to be_a Integer
    end
  end

  #############################################################################
  describe '#link_existing_rooms' do
    before :each do
      @zone   = Realm::Zone.new
      @room_a = Logistics::Builder.create_room @zone
      @room_b = Logistics::Builder.create_room @zone

      Logistics::Builder.link_existing_rooms @room_a, @room_b, 'east'
    end

    it 'sets both exits in opposing directions' do
      expect(@room_a.east_room).to be @room_b
      expect(@room_b.west_room).to be @room_a
    end
  end

  #############################################################################
  describe '#create_linked_room' do
    before :each do
      @zone   = Realm::Zone.new
      @room_a = Logistics::Builder.create_room @zone
      @room_b = Logistics::Builder.create_linked_room @room_a, 'east'
    end

    it 'returns a new Room' do
      expect(@room_b).to be_a Realm::Room
    end

    it 'adds the new Room to the Zone' do
      expect(@zone.rooms.keys).to include @room_b.id
      expect(@room_b.zone).to eq @zone
    end

    it 'sets the coordinates of the new Room' do
      expect(@room_b.coords).to eq Vector[1, 0, 0]
    end
  end

  ###########################################################################
  describe '#create_npc' do
    before :each do
      @zone = Realm::Zone.new
      @npc  = Logistics::Builder.create_npc @zone
    end

    it 'returns a new NPC' do
      expect(@npc).to be_a Realm::NPC
      expect(@npc).to be_a Realm::Being
      expect(@npc).to be_a Realm::Objekt
    end

    it 'assigns an Integral RID' do
      expect(@npc.rid).to be_a Integer
      expect(@npc.rid).to be > 0
      expect(@npc.level).to eq(1)
    end

    it 'adds the NPC to the Zone' do
      expect(@zone.npcs.values).to include(@npc)
    end
  end

  #############################################################################
  describe '#create_zone' do
    before :each do
      @zone = Logistics::Builder.create_zone
    end

    it 'returns a new Zone' do
      expect(@zone).to be_a Realm::Zone
      expect(@zone).to be_a Realm::Objekt
    end

    it 'assigns an Integral RID' do
      expect(@zone.rid).to be_a Integer
      expect(@zone.rid).to be > 0
    end
  end
end
