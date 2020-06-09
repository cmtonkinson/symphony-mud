require 'spec_helper'

include Realm

describe Realm::Avatar do
  describe '#' do
    it 'should work' do
      a = Avatar.new
      expect(true).to eq true
    end
  end
end
