require 'spec_helper'

describe Command::CommandSet do

  subject { Command::CommandSet.new }

  it { should be_a SortedSet }

end
