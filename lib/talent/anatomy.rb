module Talent
  class Anatomy
    include Talent::Enabler

    ###########################################################################
    def initialize
      @name      = ''
      @slots     = Array.new
      @abilities = Array.new
    end

    ###########################################################################
    def add_slot(type, name)
      @slots.append Mechanics::Slot.new(type, name)
    end

  end
end
