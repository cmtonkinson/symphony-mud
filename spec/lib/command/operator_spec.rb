require 'spec_helper'

include Command

describe Command::Operator do

  before :each do
    user = double('User')
    @o   = Operator.new user
  end

  describe '#handle' do

  end

end
