require 'spec_helper'

include Mechanics

describe SimpleStat do

  #############################################################################
  describe '#current' do

    before :each do
      @baseline = 20
      @stat     = SimpleStat.new @baseline
    end

    it 'shourd return the baseline by default' do
      expect(@stat.current).to eq @baseline
    end

    it 'should return no less than 0' do
      @stat.modify -100
      expect(@stat.current).to eq 0
    end

  end

  #############################################################################
  describe '#modify' do

    before :each do
      @baseline = 10
      @stat     = SimpleStat.new @baseline
    end

    it 'should increase the current value' do
      @amount = 10
      @stat.modify @amount
      expect(@stat.current).to eq @baseline + @amount
    end

  end

  #############################################################################
  describe '#unmodify' do

    before :each do
      @baseline = 10
      @stat     = SimpleStat.new @baseline
    end

    it 'should decrease the current value' do
      @amount = 5
      @stat.unmodify @amount
      expect(@stat.current).to eq @baseline - @amount
    end

  end

end
