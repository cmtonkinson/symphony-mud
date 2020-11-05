require 'spec_helper'

describe Realm::Item do
  describe '#modify' do
    before :each do
      @i = Realm::Item.new
    end

    it 'should do nothing by default' do
      @i.modify({})
      expect(@i.modifiers).to eq({})
    end

    it 'should add new keys' do
      @i.modify({ strength: 10 })
      expect(@i.modifiers).to eq({ strength: 10 })
    end

    it 'should stack on existing keys' do
      @i.modify({ focus: 5 })
      @i.modify({ focus: 7 })
      expect(@i.modifiers).to eq({ focus: 12 })
      @i.modify({ focus: -3 })
      expect(@i.modifiers).to eq({ focus: 9 })
    end

    it 'should remove modifiers brought to zero' do
      @i.modify({ luck: 2 })
      @i.modify({ luck: -2 })
      expect(@i.modifiers).to eq({})
    end

    it 'should reverse modifications' do
      @i.modify({ constitution: 1 })
      @i.modify({ constitution: 1 }, true)
      expect(@i.modifiers).to eq({})
    end

    it 'should accept multiple modifiers at once' do
      @i.modify({ charisma: 2, dexterity: 2 })
      expect(@i.modifiers).to eq({ charisma: 2, dexterity: 2 })
      @i.modify({ charisma: 2, dexterity: 2 }, true)
      expect(@i.modifiers).to eq({})
    end
  end
end
