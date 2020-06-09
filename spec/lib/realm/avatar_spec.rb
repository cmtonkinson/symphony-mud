require 'spec_helper'

describe Realm::Avatar do
  describe '#' do
    it 'should work' do
      Realm::Avatar.new
      expect(true).to eq true
    end
  end
end
