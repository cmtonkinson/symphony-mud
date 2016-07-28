require 'spec_helper'

include Command

describe Command::Usage do

  describe "#push" do

    it "should reject duplicate Argument names" do
      args = Usage.new

      arg1 = Argument.new :foo
      expect { args.push arg1 }.not_to raise_error

      arg2 = Argument.new "bar"
      expect { args.push arg2 }.not_to raise_error

      arg3 = Argument.new "foo"
      expect { args.push arg3 }.to raise_error ArgumentError
    end

    it "should reject multiple empty usages" do
      args = Usage.new

      empty1 = Argument.new
      expect { args.push empty1 }.not_to raise_error

      full1 = Argument.new "foo"
      expect { args.push full1 }.not_to raise_error

      empty2 = Argument.new
      expect { args.push empty2 }.to raise_error ArgumentError
    end

    it "should proxy Array methods to the underlying data structure" do
      args = Usage.new

      arg1 = Argument.new :foo
      args.push arg1

      arg2 = Argument.new "bar"
      args.push arg2

      expect(args.map(&:name)).to eq ["foo", "bar"]
    end

  end

end
