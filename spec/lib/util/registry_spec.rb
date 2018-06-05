require 'spec_helper'

include Util

describe Registry do

  #############################################################################
  describe '::register' do

    before :each do
      Registry.hash.clear
      expect(Registry.hash.empty?).to be true
      @a = TestObj.new
    end

    it 'adds a bucket for the Objekt type' do
      expect(Registry.hash.empty?).to be false
    end

    it 'adds the Objekt to the right bucket' do
      expect(Registry.hash.key? TestObj).to be true
      expect(Registry.hash[TestObj].key? @a.id).to be true
    end

  end

  #############################################################################
  describe '::unregister' do

    before :each do
      Registry.hash.clear
      @a = TestObj.new
      @b = TestObj.new
      expect(Registry.hash[TestObj].empty?).to be false
    end

    it 'removes the Objekt from the bucket' do
      Registry.unregister @a
      expect(Registry.hash[TestObj].key? @a.id).to be false
    end

    it 'removes the bucket itself when empty' do
      Registry.unregister @a
      Registry.unregister @b
      expect(Registry.hash.key? TestObj).to be false
    end

  end

  #############################################################################
  describe '::lookup' do

    before :each do
      Registry.hash.clear
      @a = TestObj.new
      @b = TestObj.new
      expect(Registry.hash[TestObj].empty?).to be false
    end

    it 'finds registered Objekts by id' do
      expect(Registry.lookup @a.id).to eq @a
      expect(Registry.lookup @b.id).to eq @b
      expect(Registry.lookup 'womp womp').to eq nil
    end

  end

  #############################################################################
  describe '::report' do

    before :each do
      Registry.hash.clear
      @a = TestObj.new
    end

    it 'returns the number of objects registered' do
      expect(Registry.report.key? :size).to eq true
      expect(Registry.report[:size]).to eq 1
    end

    it 'returns data on each bucket' do
      expect(Registry.report.key? :buckets).to eq true
      expect(Registry.report[:buckets][@a.class.name].key? :size).to eq true
      expect(Registry.report[:buckets][@a.class.name][:size]).to eq 1
    end

  end

end
