require_relative '../util/locatable'

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
    def self.anatomies
      ObjectSpace.each_object(::Class).select { |c| c < self }
    end

    ###########################################################################
    def setup
      being.anatomy = self
      being.add_enabler self
      being.modify modifiers if respond_to? :modifiers
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
          change_equipment primary_slot, item, container
        else
          change_equipment secondary_slot, item, container
        end
      # All other items in all other scenarios follow a generic pattern.
      else
        empty_slot = get_empty_slot item.slot
        if empty_slot
          change_equipment empty_slot, item, container
        else
          nonempty = get_slots(item.slot).first
          change_equipment nonempty, item, container
        end
      end
    end

    ###########################################################################
    def remove(item, container = nil)
      slot = slots.find { |s| s.occupant == item }
      return I18n.t('anatomy.unworn') if slot.nil?

      change_equipment slot, nil, container
    end

    ###########################################################################
    def change_equipment(slot, item, container)
      if item
        removed = slot.set item, container
        container&.delete item

        being.modify(item.modifiers)
        being.modify(removed.modifiers, true) if removed

        if removed
          [I18n.t('anatomy.replace'), removed, item]
        else
          [I18n.t('anatomy.wear'), item]
        end
      else
        removed = slot.set nil, container
        being.modify(removed.modifiers, true) if removed
        [I18n.t('anatomy.remove'), removed]
      end
    end

    ###########################################################################
    def primary_slot
      case being.anatomy
      when Human then get_slots(being.hand + ' hand').first
      end
    end

    ###########################################################################
    def secondary_slot
      case being.anatomy
      when Human then get_slots((being.hand == 'right' ? 'left' : 'right') + ' hand').first
      end
    end

    ###########################################################################
    def primary_weapon
      primary_slot&.occupant
    end

    ###########################################################################
    def secondary_weapon
      secondary_slot&.occupant
    end

    ###########################################################################
    def default_damage
      I18n.t("anatomy.default_damage.#{@name.downcase}")
    end
  end
end
