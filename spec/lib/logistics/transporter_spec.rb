require 'spec_helper'

describe Logistics::Transporter do

  before :each do
    @src  = AttrHash.new
    @dest = AttrHash.new
    @ob   = TestObj.new
  end

  #############################################################################
  describe '#add#to' do

    it 'inserts the Objekt into the target container' do
      expect(@dest.length).to eq 0
      expect(@ob.add).to be_a Logistics::Adder
      expect(@ob.add.to @dest).to be_nil
      expect(@dest.length).to eq 1
    end

  end

  describe '#remove#from' do

    it 'removes the Objekt from the target container' do
      @src.insert @ob
      expect(@src.length).to eq 1
      expect(@ob.remove).to be_a Logistics::Remover
      expect(@ob.remove.from @src).to be_nil
      expect(@src.length).to eq 0
    end

  end

  describe 'move' do

    it 'transfers the Objekt from source to destination' do
      # Start out with @ob in @src
      @src.insert @ob
      expect(@src.length).to eq 1
      expect(@dest.length).to eq 0

      # Without a `to` AND a `from`, the call does nothing.
      expect(@ob.move.from @src).to be_a Logistics::Mover
      expect(@ob.move.to @dest).to be_a Logistics::Mover
      expect(@src.length).to eq 1
      expect(@dest.length).to eq 0

      # Move the Objekt. 
      expect(@ob.move.from(@src).to(@dest)).to be_nil
      expect(@src.length).to eq 0
      expect(@dest.length).to eq 1

      # Move it back (excercising reversed from/to ordering).
      expect(@ob.move.to(@src).from(@dest)).to be_nil
      expect(@src.length).to eq 1
      expect(@dest.length).to eq 0
    end

  end

  describe 'hooks' do

    it 'should be fired around add' do
      @ob.add.to @src
      expect(@ob.before_add_ran).to be_truthy
      expect(@ob.after_add_ran).to be_truthy
      expect(@ob.before_remove_ran).to be_falsy
      expect(@ob.after_remove_ran).to be_falsy
    end

    it 'should be fired around remove' do
      @dest.insert @ob
      @ob.remove.from @dest
      expect(@ob.before_add_ran).to be_falsy
      expect(@ob.after_add_ran).to be_falsy
      expect(@ob.before_remove_ran).to be_truthy
      expect(@ob.after_remove_ran).to be_truthy
    end

    it 'should be fired around move' do
      @src.insert @ob
      @ob.move.from(@src).to(@dest)
      expect(@ob.before_add_ran).to be_truthy
      expect(@ob.after_add_ran).to be_truthy
      expect(@ob.before_remove_ran).to be_truthy
      expect(@ob.after_remove_ran).to be_truthy
    end

  end

end

