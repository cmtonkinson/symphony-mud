require 'spec_helper'

describe Command::Command do

  subject { Command::Command_banana.new }
  let(:name) { subject.name }

  it "should have a name" do
    expect(subject.get_name).to eql "banana"
  end

  describe "#<=>" do

    it { should respond_to :<=> }

    it "should sort on name" do
      expect(subject <=> Command::Command_apple.new).to   eq  1
      expect(subject <=> Command::Command_banana.new).to  eq  0
      expect(subject <=> Command::Command_cherry.new).to  eq -1
    end

  end

  describe "#execute" do

    it "should be defined" do
      expect { Command::Command_with_execute.new.execute nil, nil }.not_to raise_error
      expect { Command::Command_without_execute.new.execute nil, nil }.to raise_error NoMethodError
    end

  end

end
