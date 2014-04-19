require 'spec_helper'

describe Command::CommandSet do

  subject { Command::CommandSet.new "foo" }
  let(:name) { subject.name }

  it { should be_a SortedSet }

  it "should have a name" do
    expect(name).to eql "foo"
  end

end
