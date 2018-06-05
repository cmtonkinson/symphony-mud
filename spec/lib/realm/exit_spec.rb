require 'spec_helper'

include Realm

describe Realm::Exit do

  describe '#' do

    it 'should work' do
      e = Exit.new
      expect(true).to eq true
    end

  end

end
