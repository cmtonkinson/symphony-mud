module Talent
  module Slotted
    ###########################################################################
    def slots
      @slots ||= []
    end

    ###########################################################################
    def add_slot(slotted, type, name)
      slots.push Mechanics::Slot.new(slotted, type, name)
    end

    ###########################################################################
    def get_slots(type_or_name)
      case type_or_name
      when Symbol then slots.select { |s| s.type == type_or_name }
      when String then slots.select { |s| s.name.include? type_or_name }
      else Util.argerr "expected Symbol or String; got #{type_or_name.class.name}"
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
