require 'spec_helper'

include Mechanics

describe MaxStat do

  #############################################################################
  describe '#current' do

    before :each do
      @max_val   = 100
      @stat      = MaxStat.new @max_val
      @test_vals = [-100, -50, -1, 0, 1, 50, 100]
    end

    it 'should return no less than zero' do
      @test_vals.each do |test|
        @stat.current = test
        expect(@stat.current).to be >= 0
      end
    end

    it 'should return no more than the max' do
      @test_vals.each do |test|
        @stat.current = test
        expect(@stat.current).to be <= 100
      end
    end

  end

  #############################################################################
  describe '#max' do

    it 'should return the configured maximum' do
      stat = MaxStat.new 412
      expect(stat.max).to eq 412
    end

  end

  #############################################################################
  describe '#restore' do

    it 'should set the current value to the defined max' do
      max = 337
      val = 89
      stat = MaxStat.new max 
      stat.current = val

      expect(stat.current).to eq val
      stat.restore
      expect(stat.current).to eq max
    end

  end

end
