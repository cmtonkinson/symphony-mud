require 'spec_helper'

describe Command::Command do
  subject { Command::CommandBanana.new }
  let(:name) { subject.name }

  it 'should have a name' do
    expect(subject.get_name).to eql 'banana'
  end

  describe '#<=>' do
    it { should respond_to :<=> }

    it 'should sort on name' do
      expect(subject <=> Command::CommandApple.new).to  eq  1
      expect(subject <=> Command::CommandBanana.new).to eq  0
      expect(subject <=> Command::CommandCherry.new).to eq(-1)
    end
  end

  describe '#execute' do
    it 'should be defined' do
      expect { Command::CommandWithExecute.new.execute nil, nil }.not_to raise_error
      expect { Command::CommandWithoutExecute.new.execute nil, nil }.to raise_error NoMethodError
    end
  end
end
