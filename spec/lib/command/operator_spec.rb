require 'spec_helper'

include Command

describe Command::Operator do

  before :each do
    user = double("User")
    set  = Command.configuration.command_sets[:global]
    @o   = Operator.new user, set
  end

  describe "#handle" do

  end

end
