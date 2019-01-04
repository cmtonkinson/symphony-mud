module Scripting
  class Lua

    #############################################################################
    def initialize
      @state = nil
    end

    #############################################################################
    def allocate
      @state = Rufus::Lua::State.new 
    end

    #############################################################################
    def deallocate
      @state.close
    end 

  end
end
