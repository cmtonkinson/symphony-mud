module Talent
  class Anatomy
    include Talent::Enabler
    include Talent::Slotted
    include Util::Locatable

    attr_reader :being

    ###########################################################################
    def initialize(being)
      @name  = ''
      @being = being
    end

    ###########################################################################
    def values
      slots.map(&:occupant).compact
    end
    
    ###########################################################################
    def wear(item, container = nil)
      return I18n.t('anatomy.unwearable') unless item.wearable
      return I18n.t('anatomy.incompatible') unless get_slots(item.slot)

      # We need special handling for items in the :hold slot.
      # If it's a weapon, always place it in the primary hand. Anything else
      # should be placed in the off-hand.
      #
      # TODO: The only exception will be when a weapon is already held and the
      #       dual wield skill is learned - in that case the new weapon should
      #       go in the off-hand. But dual wield isn't written yet.
      if item.slot == :hold
        if item.type == :weapon
          removed = primary_slot.set item, container
          container.delete item
          return I18n.t('anatomy.wear'), item
        else
          removed = secondary_slot.set item, container
          container.delete item
          return I18n.t('anatomy.wear'), item
        end  
      # All other items in all other scenarios follow a generic pattern.
      else
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
    end

    ###########################################################################
    def remove(item, container = nil)
      slot = slots.find { |s| s.occupant == item }
      return I18n.t('anatomy.unworn') if slot.nil?
      removed = slot.set nil, container
      return I18n.t('anatomy.remove'), removed
    end

    ###########################################################################
    def primary_slot
      get_slots(being.hand + ' hand').first
    end

    ###########################################################################
    def secondary_slot
      get_slots((being.hand == 'right' ? 'left' : 'right') + ' hand').first
    end

    ###########################################################################
    def primary_weapon
      primary_slot.occupant
    end

    ###########################################################################
    def secondary_weapon
      secondary_slot.occupant
    end

  end
end
