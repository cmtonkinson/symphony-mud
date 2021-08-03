require 'spec_helper'

describe Realm::Reward do
  before :each do
    @empty = Realm::Reward.new
    @one = Realm::Reward.new nil, silver: 1, gold: 1, exp: 1, item_ids: [1]
    @two = Realm::Reward.new nil, silver: 2, gold: 2, exp: 2, item_ids: [2]
    @three = Realm::Reward.new nil, silver: 3, gold: 3, exp: 3, item_ids: [1, 2]
  end

  describe 'empty?' do
    it 'should return true for a newly initialized Reward' do
      expect(Realm::Reward.new.empty?).to eq(true)
    end

    it 'should return false when any value is set' do
      expect(Realm::Reward.new(nil, silver: 1).empty?).to eq(false)
      expect(Realm::Reward.new(nil, gold: 1).empty?).to eq(false)
      expect(Realm::Reward.new(nil, exp: 1).empty?).to eq(false)
      expect(Realm::Reward.new(nil, item_ids: [0]).empty?).to eq(false)
    end
  end

  describe '#+' do
    it 'should initialize an empty Reward' do
      expect(@empty.empty?).to eq(true)
    end

    it 'should not change when adding an empty Reward' do
      added = @one + @empty
      expect(added == @one).to eq(true)
    end

    it 'should add each property' do
      added = @one + @two
      expect(added == @three).to eq(true)
    end
  end
end
