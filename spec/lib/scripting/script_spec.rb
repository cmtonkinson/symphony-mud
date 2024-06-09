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
      while true do
        local a = 1;
      end
    LUA
    expect { subject.script infinite_loop }.not_to raise_error
    expect { subject.run }.to raise_error Scripting::TimeoutError
  end
end
