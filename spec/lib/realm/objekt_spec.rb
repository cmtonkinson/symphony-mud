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

  #############################################################################
  describe "#hook" do

    around :each do |example|
      class HookedObjekt < Objekt
        attr_accessor :prop
        def initialize(id = nil)
          super
          @prop = false
        end
      end
      @hooked = HookedObjekt.new

      example.run

      Object.send :remove_const, :HookedObjekt
    end

    it "should add a hook declaration method" do
      expect(@hooked).not_to respond_to :custom_hook
      class HookedObjekt
        hook :custom_hook
      end
      expect(@hooked.class).to respond_to :custom_hook
    end

    it "should allow definition of new hook callbacks" do
      expect { class HookedObjekt
        hook :custom_hook
        custom_hook -> obj { obj.prop = true }
      end }.not_to raise_error
    end

    it "should invoke callbacks when hooks are triggered" do
      class HookedObjekt
        hook :custom_hook
        custom_hook -> obj { obj.prop = true }
      end

      expect(@hooked.prop).to eq false
      @hooked.trigger :custom_hook
      expect(@hooked.prop).to eq true
    end

  end

  #############################################################################
  describe "#simple_flag" do

    around :each do |example|
      class FlaggedObjekt < Objekt
        def initialize(id = nil)
          super
        end
      end
      @unflagged = FlaggedObjekt.new

      example.run

      Object.send :remove_const, :FlaggedObjekt
    end

    it "should add an instance variable and " do
      expect(@unflagged).not_to respond_to :switch
      class FlaggedObjekt
        simple_flag :switch, false, :on?, :on!, :off?, :off!
      end
      expect(@unflagged).to respond_to :switch
      expect(@unflagged).to respond_to :switch=
    end

    it "should add an instance variable and accessors" do
      expect(@unflagged).not_to respond_to :switch
      class FlaggedObjekt
        simple_flag :switch, false, :on?, :on!, :off?, :off!
      end
      @flagged = FlaggedObjekt.new

      expect(@flagged).to have_attributes(switch: false)

      expect(@flagged).to respond_to :on?
      expect(@flagged.on?).to eq false

      expect(@flagged).to respond_to :off?
      expect(@flagged.off?).to eq true

      expect(@flagged).to respond_to :on!
      @flagged.on!
      expect(@flagged.on?).to eq true

      expect(@flagged).to respond_to :off!
      @flagged.off!
      expect(@flagged.off?).to eq true
    end

  end

end
