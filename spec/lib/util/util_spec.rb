require 'spec_helper'

describe Util do

  subject { Util }

  #############################################################################
  describe "#bound" do

    it "should return the lower bound if the given value is lesser" do
      expect(subject.bound -6, 1, 10).to eq 1
    end

    it "should return the upper bound if the given value is greater" do
      expect(subject.bound 60, 1, 10).to eq 10
    end

    it "should return values within the range" do
      expect(subject.bound 5, 1, 10).to eq 5
    end

  end

end
