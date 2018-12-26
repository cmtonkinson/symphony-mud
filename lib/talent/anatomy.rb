module Talent
  class Anatomy
    include Talent::Enabler
    include Talent::Slotted

    ###########################################################################
    def initialize
      @name = ''
    end

    ###########################################################################
    def wear(item)
      return I18n.t('anatomy.unwearable') unless item.wearable
      return I18n.t('anatomy.incompatible') unless get_slots(item.slot)

      empty = get_empty_slot item.slot
      if empty
        empty.set item
        puts I18n.t('anatomy.wear', item)
      else
        nonempty = get_slots(item.slot).first
        removed = nonempty.set item, inventory
        puts I18n.t('anatomy.replace', removed, item)
      end
    end

  end
end
