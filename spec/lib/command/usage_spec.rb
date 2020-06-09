require 'spec_helper'

describe Command::Usage do
  describe '#push' do
    it 'should reject duplicate Argument names' do
      args = Command::Usage.new

      arg1 = Command::Argument.new :foo
      expect { args.push arg1 }.not_to raise_error

      arg2 = Command::Argument.new 'bar'
      expect { args.push arg2 }.not_to raise_error

      arg3 = Command::Argument.new 'foo'
      expect { args.push arg3 }.to raise_error ArgumentError
    end

    it 'should reject multiple empty usages' do
      args = Command::Usage.new

      empty1 = Command::Argument.new
      expect { args.push empty1 }.not_to raise_error

      full1 = Command::Argument.new 'foo'
      expect { args.push full1 }.not_to raise_error

      empty2 = Command::Argument.new
      expect { args.push empty2 }.to raise_error ArgumentError
    end

    it 'should proxy Array methods to the underlying data structure' do
      args = Command::Usage.new

      arg1 = Command::Argument.new :foo
      args.push arg1

      arg2 = Command::Argument.new 'bar'
      args.push arg2

      expect(args.map(&:name)).to eq %w[foo bar]
    end
  end
end
