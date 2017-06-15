require 'spec_helper'

include Realm

describe Objekt do

  #############################################################################
  describe "#initialize" do

    it "should set a default name" do
      r = Objekt.new
      expect(r.name).to eq Objekt::DEFAULTS[:name].call
    end

  end

  #############################################################################
  describe "#fully_qualified_indentifier" do

    before :each do
      @sut = TestObj.new
      @fqid = @sut.fully_qualified_identifier
    end

    it "should return a class name (without module)" do
      expect(@fqid).to have_key :klass
      expect(@fqid[:klass]).to eq @sut.class.name.split(":").last
    end

    it "should return a name" do
      expect(@fqid).to have_key :name
      expect(@fqid[:name]).to eq @sut.name
    end

    it "should return a rid" do
      expect(@fqid).to have_key :rid
      expect(@fqid[:rid]).to eq @sut.rid
    end

    it "should return the first 8 bytes of the uuid" do
      expect(@fqid).to have_key :id
      expect(@fqid[:id]).to eq @sut.id[0..7]
    end

  end

  #############################################################################
  describe "#insert" do

    before :each do
      @bucket = TestBucket.new
      @parent = TestParent.new
      @child  = TestChild.new
    end

    it "should reject invalid object types" do
      expect { @bucket.insert OpenStruct.new }.to raise_error ArgumentError
    end

    it "should allow insertion of defined containers" do
      expect { @bucket.insert @parent }.not_to raise_error
    end

    it "should allow insertion of subclass instances to parent containers" do
      expect { @bucket.insert @child }.not_to raise_error
    end

  end

  #############################################################################
  describe "#clone" do

    before :each do
      @original = TestObj.new nil, my_num: 387, my_nil: nil
      @clone    = @original.clone
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
      %i(my_num my_nil).each do |att|
        # The values should be the same.
        expect(@clone.send(att)).to eq @original.send(att)
        # The objects should not be the same.
        unless [NilClass, Integer].include? @clone.send(att).class
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
      expect(@yaml).to include ":objekt:"
      expect(@yaml).to include "\n  :class:"
      expect(@yaml).to include "\n  :id:"
      expect(@yaml).to include "\n  :rid:"
      expect(@yaml).to include "\n  :name:"
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
      expect(@reloaded.name).to eq Objekt::DEFAULTS[:name].call
    end

    it "decodes subclass attributes" do
      expect(@reloaded.my_num).to eq 456
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
