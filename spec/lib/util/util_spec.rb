require 'spec_helper'

TOLERANCE = 0.001

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

  #############################################################################
  describe "#logistic_curve" do

    tests = {
      x: [
        [[  0, 1000, 1, 50], 1.0],
        [[  1, 1000, 1, 50], 1.147],
        [[ 10, 1000, 1, 50], 3.968],
        [[100, 1000, 1, 50], 998.999],
      ],
      limit: [
        [[100,     2, 1, 50], 1.0],
        [[100,    10, 1, 50], 9.0],
        [[100,   100, 1, 50], 99.0],
        [[100,  1000, 1, 50], 998.999],
        [[100, 10000, 1, 50], 9999.0],
      ],
      min: [
        [[1, 1000,   1, 50], 1.147],
        [[1, 1000,  10, 50], 10.952],
        [[1, 1000, 100, 50], 104.025],
        [[1, 1000, 999, 50], 998.852],
      ],
      half_max_x: [
        [[50, 1000, 1, 25], 998.999],
        [[50, 1000, 1, 50], 500.0],
        [[50, 1000, 1, 75], 90.936],
      ],
    }

    tests.each do |var, values|
      values.each do |inputs, output|
        it "should adjust for changes in #{var} (input #{inputs})" do
          expect(subject.logistic_curve *inputs).to be_within(TOLERANCE).of output
        end
      end
    end

  end

  #############################################################################
  describe "#polynomial_curve" do

    tests = {
      x: [
        [[  1, 100, 1, 1000], 1.0],
        [[ 50, 100, 1, 1000], 348.113],
        [[100, 100, 1, 1000], 1000.0],
      ],
      x_max: [
        [[10,   10, 1, 1000], 1000.0],
        [[10,  100, 1, 1000], 28.182],
        [[10, 1000, 1, 1000], 10.0],
      ],
      y_min: [
        [[50, 100,   1, 1000], 348.113],
        [[50, 100,  10, 1000], 354.462],
        [[50, 100, 100, 1000], 417.749],
      ],
      y_max: [
        [[50, 100, 1,   10], 7.429],
        [[50, 100, 1,  100], 50.0],
        [[50, 100, 1, 1000], 348.113],
      ],
    }

    tests.each do |var, values|
      values.each do |inputs, output|
        it "should adjust for changes in #{var} (input #{inputs})" do
          expect(subject.polynomial_curve *inputs).to be_within(TOLERANCE).of output
        end
      end
    end

  end

end
