module Mechanics
  class Slot
    attr_reader :occupant, :type, :name

    ###########################################################################
    def initialize(type, name)
      @occupant = nil
      @type     = type
      @name     = name
    end

    ###########################################################################
    # Track the given occupant as inhabiting this slot. If new_container is
    # given, insert the old occupant (if there was one) into it. Either way,
    # return the old occupant or nil. 
    #
    # Set occupant to nil to clear the slot (this preserves the behavior as
    # explained above for new_container).
    def set(new_occupant, new_container = nil)
      previous = occupant
      @occupant = new_occupant

      if previous && new_container
        new_container.insert previous
      end

      return previous
    end

    ###########################################################################
    def empty?
      occupant.nil?
    end

  end
end

