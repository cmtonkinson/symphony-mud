require 'spec_helper'

describe Util::AttrHash do
  it 'should be a normal Hash' do
    h = Util::AttrHash.new

    expect(h).to be_a Hash

    expect(h.empty?).to eq true
    expect(h.keys).to eq []
    expect(h.values).to eq []

    h['foo'] = 5
    expect(h.empty?).to eq false
    expect(h.keys).to eq ['foo']
    expect(h.values).to eq [5]
  end

  describe '#initialize' do
    it 'should default the key attribute to :id' do
      h = Util::AttrHash.new
      expect(h.attribute).to eq :id
    end

    it 'should allow overwriting the key attribute' do
      h = Util::AttrHash.new :baz
      expect(h.attribute).to eq :baz
    end
  end

  describe '#insert' do
    it 'should attempt insertion by the configured attribute' do
      h  = Util::AttrHash.new :qux
      s1 = OpenStruct.new foo: 'a', bar: 'b'
      s2 = OpenStruct.new baz: 'c', qux: 'd'

      expect { h.insert s1 }.to raise_error ArgumentError
      expect { h.insert s2 }.not_to raise_error

      expect(h.size).to eq 1
      expect(h.empty?).to eq false
      expect(h.keys).to eq ['d']
      expect(h.values).to eq [s2]
    end
  end

  describe '#remove' do
    it 'should remove by key of the configured attribute' do
      h = Util::AttrHash.new :foo
      s = OpenStruct.new foo: 'a', bar: 'b'

      expect(h.size).to eq 0
      expect { h.insert s }.not_to raise_error
      expect(h.size).to eq 1
      expect { h.delete s }.not_to raise_error
      expect(h.size).to eq 0
    end
  end

  describe '#locate' do
    before :each do
      @h = Util::AttrHash.new
      ['sword', 'sword', 'crystal', 'sword',
       'mushroom', 'small cup', 'mutton',
       'little pink flower'].each_with_index do |name, i|
        @h.insert Realm::TestItem.new nil, name: name, rid: i
      end
    end

    describe 'in the positive case' do
      it 'should find a single Objekt by exact name' do
        found = @h.locate 'crystal'
        expect(found).to be_an Array
        expect(found).to have_exactly(1).items
        expect(found[0].name).to eq 'crystal'

        found = @h.locate "'mutton'"
        expect(found).to be_an Array
        expect(found).to have_exactly(1).items
        expect(found[0].name).to eq 'mutton'
      end

      it 'should find all Objekts by exact name' do
        found = @h.locate 'sword*all'
        expect(found).to have_exactly(3).items
        expect(found.map(&:name)).to contain_exactly('sword', 'sword', 'sword')
      end

      it 'should find an Objekt by name prefix' do
        found = @h.locate 'mu'
        expect(found).to have_exactly(1).items
        expect(found[0].name).to eq 'mushroom'
      end

      it 'should find no more than N Objekts by quantity' do
        found = @h.locate 'sword*2'
        expect(found).to have_exactly(2).items
        expect(found.map(&:name)).to contain_exactly('sword', 'sword')
      end

      it 'should find an Objekt by index' do
        found = @h.locate 'sword#2'
        expect(found).to have_exactly(1).items
        expect(found[0].rid).to eq 1
      end

      it 'should find Objekts by multi-word name' do
        found = @h.locate "'small cup'"
        expect(found).to have_exactly(1).items
        expect(found[0].name).to eq 'small cup'
      end

      it 'should find Objekts by subsequent terms' do
        found = @h.locate 'pink'
        expect(found).to have_exactly(1).items
        expect(found[0].name).to eq 'little pink flower'
      end

      # TODO: nice to have
      # it "should find multiple Objekts by multiple names" do
      #  found = @h.locate "sword mushroom 'small cup'"
      #  expect(found).to have_exactly(3).items
      #  expect(found.map &:name).to contain_exactly ["sward", "mushroom", "small cup"]
      # end
    end

    describe 'in the negative case' do
      it 'should not return more Objekts than are available' do
        found = @h.locate 's*100'
        expect(found).to have_exactly(4).items
      end

      it 'should return empty when the index exceeds the number of matching names' do
        found = @h.locate 'sword#4'
        expect(found).to have_exactly(0).items
      end

      it 'should return empty when there is no prefix match' do
        found = @h.locate 'nadda'
        expect(found).to have_exactly(0).items
      end
    end
  end
end
