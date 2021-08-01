require 'spec_helper'

describe Realm::Being do
  describe 'Stat' do
    describe 'level' do
      before :each do
        @b = Realm::Being.new
      end

      it 'defaults to 1' do
        expect(@b.level).to eq 1
      end

      (Realm::Being::LEVELS - %w[NEWBIE]).each do |lvl|
        it "has a helper for level #{lvl} called :#{lvl.downcase}?" do
          @b.send :instance_variable_set, '@level', Realm::Being.const_get(lvl) - 1
          expect(@b.send("#{lvl.downcase}?")).to be false
          @b.send :instance_variable_set, '@level', Realm::Being.const_get(lvl) + 1
          expect(@b.send("#{lvl.downcase}?")).to be true
        end
      end
    end

    ###########################################################################
    describe 'resistance' do
      describe '#modify' do
        before :each do
          @b = Realm::Being.new
        end

        it 'should default to empty' do
          expect(@b.resistances).to eq({})
        end

        it 'should work with an empty Hash' do
          @b.modify({})
          expect(@b.resistances).to eq({})
        end

        it 'should apply resistances' do
          @b.modify({ fire: -5 })
          expect(@b.resistances).to eq({ fire: 95 })

          @b.modify({ water: 42 })
          expect(@b.resistances).to eq({ fire: 95, water: 142 })
        end

        it 'should remove neutral entries' do
          @b.modify({ arcane: -20 })
          @b.modify({ arcane: 20 })
          expect(@b.resistances).to eq({})
        end

        it 'should be reversible' do
          @b.modify({ fell: 24 })
          @b.modify({ fell: 24 }, true)
          expect(@b.resistances).to eq({})
        end
      end

      describe '#effective_resistance' do
        before :each do
          @b = Realm::Being.new
        end

        it 'should return at most 200' do
          @b.modify({ divine: 4302 })
          expect(@b.effective_resistance(:divine)).to eq(200)
        end

        it 'should return at least -100' do
          @b.modify({ air: -432 })
          expect(@b.effective_resistance(:air)).to eq(-100)
        end
      end
    end
  end
end
