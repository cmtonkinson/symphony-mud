module Mechanics
  class Slot
    attr_reader :slotted, :occupant, :type, :name

    def initialize(slotted, type, name)
      @occupant = nil
      @slotted  = slotted
      @type     = type
      @name     = name
    end

    # Track the given occupant as inhabiting this slot. If new_container is
    # given, insert the old occupant (if there was one) into it. Either way,
    # return the old occupant or nil.
    #
    # Set occupant to nil to clear the slot (this preserves the behavior as
    # explained above for new_container).
    def set(new_occupant, new_container = nil)
      # Save a reference to the old occupant and set the new one.
      previous  = occupant
      @occupant = new_occupant

      # If there is a new occupant, link to to the 'slotted'
      new_occupant&.slotted_to = slotted

      # If there was an occupant removed...
      if previous
        # ... disassociate it with the 'slotted'
        previous.slotted_to = nil
        # ... add it to the other container, if provided
        new_container&.insert previous
      end

      # Return previous
      previous
    end

    def empty?
      occupant.nil?
    end
  end
end
