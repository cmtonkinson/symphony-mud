require 'spec_helper'

include Realm

describe Objekt do

  #############################################################################
  describe "#initialize" do

    it "should set a default name" do
      r = Objekt.new
      expect(r.name).to eq Objekt::DEFAULTS[:name]
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
  describe "#clone" do

    before :each do
      @original = TestObj.new
      @clone = @original.clone
    end

    it "assigns a new id" do
      expect(@clone.id).not_to eq @original.id
    end

    it "sets the cloned_from attribute" do
      expect(@clone.cloned_from).to eq @original.id
    end

    it "deep copies base class Objekt attributes" do
      %i(name).each do |att|
        # The values should be the same.
        expect(@clone.send(att)).to eq @original.send(att)
        # The objects should not be the same.
        expect(@clone.send(att)).not_to be @original.send(att)
      end
    end

    it "deep copies subclass attributes" do
      %i(my_num).each do |att|
        # The values should be the same.
        expect(@clone.send(att)).to eq @original.send(att)
        # The objects should not be the same.
        unless @clone.send(att).is_a? NilClass
          expect(@clone.send(att)).not_to be @original.send(att)
        end
      end
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
      expect(@yaml).to include "Realm::TestObj"
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
      expect(@reloaded.name).to eq Objekt::DEFAULTS[:name]
    end

    it "decodes subclass attributes" do
      expect(@reloaded.my_num).to eq 456
    end

  end

end
