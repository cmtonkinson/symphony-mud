require 'spec_helper'

include Util

describe Util::AttrHash do

  #############################################################################
  it "should be a normal Hash" do
    h = AttrHash.new

    expect(h).to be_a Hash

    expect(h.empty?).to eq true
    expect(h.keys).to eq []
    expect(h.values).to eq []

    h["foo"] = 5
    expect(h.empty?).to eq false
    expect(h.keys).to eq ["foo"]
    expect(h.values).to eq [5]
  end

  #############################################################################
  describe "#initialize" do

    it "should default the key attribute to :id" do
      h = AttrHash.new
      expect(h.attribute).to eq :id
    end

    it "should allow overwriting the key attribute" do
      h = AttrHash.new :baz
      expect(h.attribute).to eq :baz
    end

  end

  #############################################################################
  describe "#insert" do

    it "should attempt insertion by the configured attribute" do
      h  = AttrHash.new      :qux
      s1 = OpenStruct.new foo: "a", bar: "b"
      s2 = OpenStruct.new baz: "c", qux: "d"

      expect { h.insert s1 }.to raise_error ArgumentError
      expect { h.insert s2 }.not_to raise_error

      expect(h.size).to eq 1
      expect(h.empty?).to eq false
      expect(h.keys).to eq ["d"]
      expect(h.values).to eq [s2]
    end

    it "should be aliased as :<<" do
      h = AttrHash.new :qux
      s2 = OpenStruct.new baz: "c", qux: "d"
      expect(h.respond_to?(:<<)).to eq true
      expect { h << s2 }.not_to raise_error
      expect(h.size).to eq 1
    end

  end

  #############################################################################
  describe "#remove" do

    it "should remove by key of the configured attribute" do
      h = AttrHash.new :foo
      s = OpenStruct.new foo: "a", bar: "b"

      expect(h.size).to eq 0
      expect { h.insert s }.not_to raise_error
      expect(h.size).to eq 1
      expect { h.remove s }.not_to raise_error
      expect(h.size).to eq 0
    end

  end

end
