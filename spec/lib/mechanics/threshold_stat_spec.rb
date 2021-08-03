require 'spec_helper'

describe Mechanics::ThresholdStat do
  describe '#percent' do
    it 'should return the ratio of the current value to the threshold' do
      stat = Mechanics::ThresholdStat.new 15, 20
      expect(stat.percent).to eq 0.75

      stat = Mechanics::ThresholdStat.new 15, 30
      expect(stat.percent).to eq 0.5

      stat = Mechanics::ThresholdStat.new 4, 20
      expect(stat.percent).to eq 0.2

      stat = Mechanics::ThresholdStat.new 40, 20
      expect(stat.percent).to eq 2.0
    end
  end
end
