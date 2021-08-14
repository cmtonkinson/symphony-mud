require 'spec_helper'

# This is as much an exercise of the Spy class as it is of Objective itself

describe Realm::Objective do
  describe '#setup' do
    before :each do
      @q = Realm::Quest.new
      @q.hero = Realm::Objekt.new
      @t = Realm::Task.new
      @t.quest = @q
      @o = Realm::Objective.new
      @o.task = @t
      @t.objective = @o
    end

    it 'should return false without a valid label' do
      expect(@o.setup(:wrong)).to eq(false)
    end

    it 'should set the appropriate trigger' do
      expect(@o.trigger).to eq(nil)
      @o.setup :visit_room, 2
      expect(@o.trigger).to eq(:visit_room)
    end

    it 'should set trigger parameters correctly' do
      expect(@o.params).to eq(nil)
      @o.setup :visit_room, 2
      expect(@o.params).to eq(2)
    end

    it 'should record failure' do
      expect(@o.failed?).to eq(false)
      @o.fail!
      expect(@o.failed?).to eq(true)
    end

    it 'should record success' do
      expect(@o.complete?).to eq(false)
      @o.complete!
      expect(@o.complete?).to eq(true)
    end
  end
end
