require 'spec_helper'

include World

describe Objekt do

  #############################################################################
  describe "#initialize" do

    it "should set a default name" do
      r = Objekt.new
      expect(r.name).to eq "_name"
    end

  end

  #############################################################################
  describe "#insert" do

    it "should reject invalid object types" do
      r = Objekt.new
      expect { r.insert OpenStruct.new }.to raise_error ArgumentError
    end

  end

  #############################################################################
  describe "#serialize and #deserialize" do

    before :each do
      @original = TestObj.new 42
      @yaml     = @original.serialize
      @reloaded = TestObj.deserialize @yaml
    end

    it "seriailizes to valid YAML" do
      expect { YAML.load @yaml }.not_to raise_error
    end

    it "encodes base Objekt attributes" do
      expect(@yaml).to include "World::TestObj"
      expect(@yaml).to include ":objekt_id"
      expect(@yaml).to include ":objekt_name"
    end

    it "encodes subclass attributes" do
      expect(@yaml).to include ":my_num"
      expect(@yaml).to include "456"
    end

    it "deseriailizes YAML" do
      expect { Objekt.deserialize @yaml }.not_to raise_error
    end

    it "decodes base Objekct attributes" do
      expect(@reloaded.id).to eq 42
      expect(@reloaded.name).to eq "_name"
    end

    it "decodes subclass attributes" do
      expect(@reloaded.my_num).to eq 456
    end

  end

end
