require 'spec_helper'

describe Realm::Effect do
  describe '#to_s' do
    it 'should print all modifiers' do
      e1 = Realm::Effect.new(nil, { name: 'nothing' })
      expect(Base::Terminal.uncolor(e1.to_s)).to eq 'nothing'

      e2 = Realm::Effect.new(nil, {
                               name: 'rage',
                               modifiers: {
                                 strength: 1,
                                 dexterity: -1
                               }
                             })
      expect(Base::Terminal.uncolor(e2.to_s)).to eq 'rage (strength +1, dexterity -1)'
    end
  end
end
