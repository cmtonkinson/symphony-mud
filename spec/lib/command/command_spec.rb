require 'spec_helper'

include Command

describe Command::Command do

  subject { Command_banana.new }
  let(:name) { subject.name }

  it 'should have a name' do
    expect(subject.get_name).to eql 'banana'
  end

  describe '#<=>' do

    it { should respond_to :<=> }

    it 'should sort on name' do
      expect(subject <=> Command_apple.new).to  eq  1
      expect(subject <=> Command_banana.new).to eq  0
      expect(subject <=> Command_cherry.new).to eq(-1)
    end

  end

  describe '#execute' do

    it 'should be defined' do
      expect { Command_with_execute.new.execute nil, nil }.not_to raise_error
      expect { Command_without_execute.new.execute nil, nil }.to raise_error NoMethodError
    end

  end

end
