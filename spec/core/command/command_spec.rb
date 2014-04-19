require 'spec_helper'

describe Command::Command do

  subject { Command::Command.new "banana" }
  let(:name) { subject.name }

  it "should have a name" do
    expect(name).to eql "banana"
  end

  context "#<=>" do

    it { should respond_to :<=> }

    it "should sort on name" do
      expect(subject <=> Command::Command.new("apple")).to eq 1
      expect(subject <=> Command::Command.new("banana")).to eq  0
      expect(subject <=> Command::Command.new("cherry")).to eq -1
    end

  end

end
