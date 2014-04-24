require 'spec_helper'

####################################################################################################
# Queue
####################################################################################################
describe Util::Queue do

  subject { Util::Queue.new }

  describe "#push" do
    it "should return self" do
      expect(subject.push 1).to be subject
    end
  end

  describe "#pop" do
    context "when empty" do
      it "should return false" do
        expect(subject.pop).to eq false
      end
    end
    context "when non-empty" do
      before :each do
        subject.push(1).push(2)
      end
      it "should remove and return elements in FIFO order" do
        expect(subject.pop).to eq 1
        expect(subject.pop).to eq 2
      end
    end
  end

  describe "#peek" do
    context "when empty" do
      it "should return false" do
        expect(subject.peek).to eq false
      end
    end
    context "when non-empty" do
      before :each do
        subject.push(1).push(2)
      end
      it "should return the top element without removing it" do
        expect(subject.peek).to eq 1
        expect(subject.peek).to eq 1
      end
    end
  end

  describe "#empty" do
    context "when empty" do
      it "should return true" do
        expect(subject.empty?).to eq true
      end
    end
    context "when non-empty" do
      before :each do
        subject.push(1).push(2)
      end
      it "should return false" do
        expect(subject.empty?).to eq false
      end
    end
  end

end
