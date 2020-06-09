require 'spec_helper'

describe Realm::Exit do
  describe '#' do
    it 'should work' do
      Realm::Exit.new
      expect(true).to eq true
    end
  end
end
