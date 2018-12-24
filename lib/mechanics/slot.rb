module Mechanics
  class Slot
    attr_reader :item, :type, :name

    ###########################################################################
    def initialize(type, name)
      @item = nil
      @type = type
      @name = name
    end

    ###########################################################################
    # Track the given item as being equipped in this slot, and return the
    # previously equipped item or nil.
    # Equip nil to clear the slot and return what was there.
    def equip(item)
      unequipped = @item
      @item = item
      unequipped
    end

  end
end

