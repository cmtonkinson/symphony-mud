require 'spec_helper'

describe Scripting::Script do
  subject { Scripting::Script.new }

  it 'should compile and run a basic script' do
    basic_script = <<-LUA
      local x = 4;
      local y = 5;
      local z = x + y;
    LUA
    expect { subject.script basic_script }.not_to raise_error
    expect { subject.run }.not_to raise_error
  end

  it 'should not allow access to OS and I/O functions' do
    [
      "os.execute('ls')",
      "io.open('file.txt', 'w')",
      "dofile('file.lua')",
      "loadfile('file.lua')",
      "print('Hello, world!')"
    ].each do |bad_script|
      expect { subject.script bad_script }.not_to raise_error
      expect { subject.run }.to raise_error Scripting::RuntimeError
    end
  end

  it 'should limit execution time' do
    infinite_loop = <<-LUA
      local x = 0;
      for i = 1, 20000000 do
        x = x + 1;
      end
    LUA
    expect { subject.script infinite_loop }.not_to raise_error
    expect { subject.run }.to raise_error Scripting::TimeoutError
  end

  it 'should not return anything by default' do
    basic_script = <<-LUA
      local x = 4;
    LUA
    expect { subject.script basic_script }.not_to raise_error
    expect { subject.run }.not_to raise_error

    return_hash = subject.result
    expect(return_hash).to be_a Hash
    expect(return_hash).to be_empty
  end

  it 'should allow the script to return values' do
    return_script = <<-LUA
      _symphony_return['one'] = 9;
      _symphony_return['two'] = true;
      _symphony_return['three'] = "test";
    LUA
    expect { subject.script return_script }.not_to raise_error
    expect { subject.run }.not_to raise_error

    return_hash = subject.result
    expect(return_hash).to be_a Hash
    expect(return_hash.size).to eq 3

    expect(return_hash['one']).to be_a Numeric
    expect(return_hash['one']).to eq 9

    expect(return_hash['two']).to eq true

    expect(return_hash['three']).to be_a String
    expect(return_hash['three']).to eq 'test'
  end
end
