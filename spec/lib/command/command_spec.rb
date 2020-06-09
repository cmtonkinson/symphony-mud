require 'spec_helper'

include Command

describe Command::Command do
  subject { CommandBanana.new }
  let(:name) { subject.name }

  it 'should have a name' do
    expect(subject.get_name).to eql 'banana'
  end

  describe '#<=>' do
    it { should respond_to :<=> }

    it 'should sort on name' do
      expect(subject <=> CommandApple.new).to  eq  1
      expect(subject <=> CommandBanana.new).to eq  0
      expect(subject <=> CommandCherry.new).to eq(-1)
    end
  end

  describe '#execute' do
    it 'should be defined' do
      expect { CommandWithExecute.new.execute nil, nil }.not_to raise_error
      expect { CommandWithoutExecute.new.execute nil, nil }.to raise_error NoMethodError
    end
  end
end
