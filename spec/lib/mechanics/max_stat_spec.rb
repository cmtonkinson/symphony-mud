require 'spec_helper'

include Mechanics

describe MaxStat do

  #############################################################################
  describe "#current" do

    before :each do
      @max_val   = 100
      @stat      = MaxStat.new @max_val
      @test_vals = [-100, -50, -1, 0, 1, 50, 100]
    end

    it "should return no less than zero" do
      @test_vals.each do |test|
        @stat.current = test
        expect(@stat.current).to be >= 0
      end
    end

    it "should return no more than the max" do
      @test_vals.each do |test|
        @stat.current = test
        expect(@stat.current).to be <= 100
      end
    end

  end

  #############################################################################
  describe "#max" do

    it "should return the configured maximum" do
      stat = MaxStat.new 412
      expect(stat.max).to eq 412
    end

  end

  #############################################################################
  describe "#bound" do

    it "should return the lower bound if the given value is lesser" do
      expect(MaxStat.bound -6, 1, 10).to eq 1
    end

    it "should return the upper bound if the given value is greater" do
      expect(MaxStat.bound 60, 1, 10).to eq 10
    end

    it "should return values within the range" do
      expect(MaxStat.bound 5, 1, 10).to eq 5
    end

  end

end
