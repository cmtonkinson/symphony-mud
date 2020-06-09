require 'spec_helper'

include Command

describe Command::Argument do
  describe '#study' do
    it 'should parse a string' do
      arg = Argument.new 'foo'
      expect(arg.name).to eq 'foo'
      expect(arg.type).to eq :literal
    end

    it 'should parse a symbol' do
      arg = Argument.new :bar
      expect(arg.name).to eq 'bar'
      expect(arg.type).to eq :id
    end

    it 'should parse an array of values' do
      arg = Argument.new x: %w[to from]
      expect(arg.name).to eq 'x'
      expect(arg.type).to eq :set
      expect(arg.value).to eq %w[to from]
    end

    it 'should reject complex Array values' do
      expect { Argument.new ['to', 'from', { jason: :bourne }] }.to raise_error ArgumentError
      expect { Argument.new ['to', 'from', %i[jason bourne]] }.to raise_error ArgumentError
    end

    it 'should parse a Hash' do
      arg = Argument.new({ confirmation: :bool })
      expect(arg.name).to eq 'confirmation'
      expect(arg.type).to eq :bool
    end
  end
end
