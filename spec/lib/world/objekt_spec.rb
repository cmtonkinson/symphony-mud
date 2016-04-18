require 'spec_helper'

describe World::Objekt do

  describe "#initialize" do
    it "should set a default name" do
      r = World::Objekt.new
      expect(r.name).to eq "_name"
    end
  end

  describe "#insert" do
    it "should reject invalid object types" do
      r = World::Objekt.new
      expect { r.insert OpenStruct.new }.to raise_error ArgumentError
    end
  end

end
