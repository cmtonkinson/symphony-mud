require 'spec_helper'

describe Util::Registry do
  #############################################################################
  describe '::register' do
    before :each do
      Util::Registry.hash.clear
      expect(Util::Registry.hash.empty?).to be true
      @a = Realm::TestObj.new
    end

    it 'adds a bucket for the Objekt type' do
      expect(Util::Registry.hash.empty?).to be false
    end

    it 'adds the Objekt to the right bucket' do
      expect(Util::Registry.hash.key?(Realm::TestObj)).to be true
      expect(Util::Registry.hash[Realm::TestObj].key?(@a.id)).to be true
    end
  end

  #############################################################################
  describe '::unregister' do
    before :each do
      Util::Registry.hash.clear
      @a = Realm::TestObj.new
      @b = Realm::TestObj.new
      expect(Util::Registry.hash[Realm::TestObj].empty?).to be false
    end

    it 'removes the Objekt from the bucket' do
      Util::Registry.unregister @a
      expect(Util::Registry.hash[Realm::TestObj].key?(@a.id)).to be false
    end

    it 'removes the bucket itself when empty' do
      Util::Registry.unregister @a
      Util::Registry.unregister @b
      expect(Util::Registry.hash.key?(Realm::TestObj)).to be false
    end
  end

  #############################################################################
  describe '::lookup' do
    before :each do
      Util::Registry.hash.clear
      @a = Realm::TestObj.new
      @b = Realm::TestObj.new
      expect(Util::Registry.hash[Realm::TestObj].empty?).to be false
    end

    it 'finds registered Objekts by id' do
      expect(Util::Registry.lookup(@a.id)).to eq @a
      expect(Util::Registry.lookup(@b.id)).to eq @b
      expect(Util::Registry.lookup('womp womp')).to eq nil
    end
  end

  #############################################################################
  describe '::report' do
    before :each do
      Util::Registry.hash.clear
      @a = Realm::TestObj.new
    end

    it 'returns the number of objects registered' do
      expect(Util::Registry.report.key?(:size)).to eq true
      expect(Util::Registry.report[:size]).to eq 1
    end

    it 'returns data on each bucket' do
      expect(Util::Registry.report.key?(:buckets)).to eq true
      expect(Util::Registry.report[:buckets][@a.class.name].key?(:size)).to eq true
      expect(Util::Registry.report[:buckets][@a.class.name][:size]).to eq 1
    end
  end
end
