require 'spec_helper'

describe Util::AttrHash do

  it "should be a normal Hash" do
    ah = Util::AttrHash.new

    expect(ah).to be_a Hash

    expect(ah.empty?).to eq true
    expect(ah.keys).to eq []
    expect(ah.values).to eq []

    ah["foo"] = 5
    expect(ah.empty?).to eq false
    expect(ah.keys).to eq ["foo"]
    expect(ah.values).to eq [5]
  end

  describe "#initialize" do

    it "should default the key attribute to :id" do
      ah = Util::AttrHash.new
      expect(ah.attribute).to eq :id
    end

    it "should allow overwriting the key attribute" do
      ah = Util::AttrHash.new :baz
      expect(ah.attribute).to eq :baz
    end

  end

  describe "#insert" do

    it "should attempt insertion by the configured attribute" do
      @ah = Util::AttrHash.new :qux
      s1 = OpenStruct.new foo: "a", bar: "b"
      s2 = OpenStruct.new baz: "c", qux: "d"

      expect { @ah.insert s1 }.to raise_error ArgumentError
      expect { @ah.insert s2 }.not_to raise_error

      expect(@ah.size).to eq 1
      expect(@ah.empty?).to eq false
      expect(@ah.keys).to eq ["d"]
      expect(@ah.values).to eq [s2]
    end

    it "should be aliased as :<<" do
      @ah = Util::AttrHash.new :qux
      s2 = OpenStruct.new baz: "c", qux: "d"
      expect(@ah.respond_to?(:<<)).to eq true
      expect { @ah << s2 }.not_to raise_error
      expect(@ah.size).to eq 1
    end

  end

end
