module Talent
  module Slotted

    def slots
      @slots ||= Array.new
    end

    ###########################################################################
    def add_slot(type, name)
      slots.push Mechanics::Slot.new(type, name)
    end

    ###########################################################################
    def get_slots(type_or_name)
      case type_or_name
      when Symbol
        slots.select { |s| s.type == type_or_name }
      when String
        slots.select { |s| s.name.include? type_or_name }
      else
        raise ArgumentError.new "Talent::Slotted#get_slots() expects Symbol or String - '#{type_or_name.class.name}' given."
      end
    end

    ###########################################################################
    def get_empty_slot(type_or_name)
      get_slots(type_or_name).select(&:empty?).first
    end

    ###########################################################################
    def valid_slot_type?(type)
      slots.any? { |s| s.type == type.to_sym }
    end

  end
end
