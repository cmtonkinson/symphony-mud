require 'spec_helper'

include Realm

describe Realm::Being do
  describe 'Stat' do

    describe 'level' do

      before :each do
        @b = Realm::Being.new
      end

      it 'defaults to 1' do
        expect(@b.level).to eq 1
      end

      (Realm::Being::LEVELS - %w(NEWBIE)).each do |lvl|
        it "has a helper for level #{lvl} called :#{lvl.downcase}?" do
          @b.send :instance_variable_set, '@level', Realm::Being.const_get(lvl) - 1
          expect(@b.send "#{lvl.downcase}?").to be false
          @b.send :instance_variable_set, '@level', Realm::Being.const_get(lvl) + 1
          expect(@b.send "#{lvl.downcase}?").to be true
        end
      end

    end

  end
end

