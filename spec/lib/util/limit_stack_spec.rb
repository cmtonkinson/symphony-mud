require 'spec_helper'

include Util

describe Util::LimitStack do

  test_limit = 5

  subject { Util::LimitStack.new(test_limit) }

  #############################################################################
  describe '#imit' do
    it 'should return the configured limit' do
      expect(subject.limit).to eq test_limit
    end
  end

  #############################################################################
  describe '#push' do
    it 'should return self' do
      expect(subject.push 1).to be subject
      expect(subject.length).to eq 1
    end

    it 'should limit the number of elements' do
      (1..10).each do |i|
        subject.push i
        expect(subject.length).to be <= subject.limit
      end

      (6..10).each do |i|
        expect(subject.pop).to eq i
      end
    end
  end

  #############################################################################
  describe '#pop' do
    context 'when empty' do
      it 'should return false' do
        expect(subject.pop).to eq false
      end
    end
    context 'when non-empty' do
      before :each do
        subject.push(1).push(2)
      end
      it 'should remove and return elements in FIFO order' do
        expect(subject.pop).to eq 1
        expect(subject.pop).to eq 2
      end
    end
  end

  #############################################################################
  describe '#peek' do
    context 'when empty' do
      it 'should return false' do
        expect(subject.peek).to eq false
      end
    end
    context 'when non-empty' do
      before :each do
        subject.push(1).push(2)
      end
      it 'should return the top element without removing it' do
        expect(subject.peek).to eq 1
        expect(subject.peek).to eq 1
      end
    end
  end

  #############################################################################
  describe '#empty' do
    context 'when empty' do
      it 'should return true' do
        expect(subject.empty?).to eq true
      end
    end
    context 'when non-empty' do
      before :each do
        subject.push(1).push(2)
      end
      it 'should return false' do
        expect(subject.empty?).to eq false
      end
    end
  end

end
