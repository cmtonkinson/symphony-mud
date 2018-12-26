module Talent
  class Anatomy
    include Talent::Enabler
    include Talent::Slotted
    include Util::Locatable

    ###########################################################################
    def initialize
      @name = ''
    end

    ###########################################################################
    def values
      slots.map(&:occupant).compact
    end
    
    ###########################################################################
    def wear(item, container = nil)
      return I18n.t('anatomy.unwearable') unless item.wearable
      return I18n.t('anatomy.incompatible') unless get_slots(item.slot)

      empty = get_empty_slot item.slot
      if empty
        empty.set item
        container.delete item
        return I18n.t('anatomy.wear'), item
      else
        nonempty = get_slots(item.slot).first
        removed = nonempty.set item, container
        container.delete item
        return I18n.t('anatomy.replace'), removed, item
      end
    end

    ###########################################################################
    def remove(item, container = nil)
      slot = slots.find { |s| s.occupant == item }
      return I18n.t('anatomy.unworn') if slot.nil?
      removed = slot.set nil, container
      return I18n.t('anatomy.remove'), removed
    end

  end
end
