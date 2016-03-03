require 'spec_helper'

describe Command::Argument do

  describe "#parse" do

    it "should parse a String" do
      arg = Command::Argument.new "foo"
      expect(arg.name).to eq "foo"
      expect(arg.type).to eq :string
    end

    it "should parse a Symbol" do
      arg = Command::Argument.new :bar
      expect(arg.name).to eq "bar"
      expect(arg.type).to eq :id
    end

    it "should parse an Array of values" do
      arg = Command::Argument.new ["to", "from"]
      expect(arg.name).to eq "anonymous"
      expect(arg.type).to eq :array
      expect(arg.value).to eq ["to", "from"]
    end

    it "should reject complex Array values" do
      expect { Command::Argument.new ["to", "from", {jason: :bourne}] }.to raise_error ArgumentError
      expect { Command::Argument.new ["to", "from", [:jason, :bourne]] }.to raise_error ArgumentError
    end

    it "should parse a Hash" do
      arg = Command::Argument.new({confirmation: :bool})
      expect(arg.name).to eq "confirmation"
      expect(arg.type).to eq :bool
    end

  end

end
