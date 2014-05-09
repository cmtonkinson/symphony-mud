require 'spec_helper'

describe Command::Operator do

  before :each do
    user = double("User")
    set  = Command.configuration.command_sets[:global]
    @o   = Command::Operator.new user, set
  end

  describe "#handle" do

  end

end
