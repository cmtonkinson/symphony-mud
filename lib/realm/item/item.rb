module Realm
  class Item < Objekt
    Konst::ITEM_ATTRS.each { |name| attr_accessor name }
    attr_accessor :slotted_to, :modifiers

    def initialize(id = nil, props = {})
      super
    end

    after_initialize lambda {
      @level         = 1
      @type          = :other
      @wearable      = false
      @slot          = nil
      @health        = 0
      @mana          = 0
      @max_occupants = 0
      @opens_type    = :room
      @opens_rid     = 0
      @weapon_type   = :club
      @damage_type   = :bash
      @element_type  = nil
      @element_ratio = 0
      @hitroll       = 0
      @damroll       = 0
      @modifiers     = {}
    }

    on_hashify lambda {
      hash = {}
      Konst::ITEM_ATTRS.each { |name| hash[name] = send name }
      hash[:modifiers] = @modifiers
      hash
    }

    on_dehashify lambda { |hash|
      Konst::ITEM_ATTRS.each { |name| instance_variable_set "@#{name}", hash[name] }
      @modifiers = hash[:modifiers]
    }

    on_deep_copy lambda { |original|
      Konst::ITEM_ATTRS.each { |name| instance_variable_set "@#{name}", original.send(name) }
      @modifiers = Marshal.load(Marshal.dump(original.modifiers))
    }

    enervate lambda {
      dispose
    }

    def _interpret_name
      '$item'
    end

    def honable?
      type == :weapon && %w[pierce slash].include?(damage_type)
    end

    def set_element(type = nil, ratio = 0)
      @element_type = type
      @element_ratio = Util::Computation.bound (type.nil? ? 0 : ratio),
                                               Konst::ITEM_ELEMENT_MIN,
                                               Konst::ITEM_ELEMENT_MAX
    end

    def element_str
      element_type ? ('%s %d%%' % [element_type, element_ratio]) : ''
    end

    def modify(mods, reverse = false)
      mods.each do |stat, magnitude|
        amount = magnitude * (reverse ? -1 : 1)
        if modifiers.key? stat
          modifiers[stat] += amount
        else
          modifiers[stat] = amount
        end
      end
      modifiers.delete_if { |_, v| v.zero? }
    end

    def summary
      str = $templates.item_summary % [
        rid, name,
        id,
        level, type,
        *(wearable ? ['yes', slot.to_s] : ['no', 'N/A']),
        (element_str == '' ? 'N/A' : element_str)
      ]
      if type == :weapon
        str << $templates.item_weapon_summary % [
          weapon_type, damage_type, hitroll, damroll
        ]
      end
      if wearable && modifiers.any?
        str << "{xModfiers:\n"
        modifiers.each do |name, amount|
          str << "{x %-15s %s%d{x\n" % [name.to_s, amount.positive? ? '{G+' : '{R', amount]
        end
      end
      if effects.any?
        str << "{xEffects:\n  "
        str << effects.map(&:to_s).join("\n  ")
        str << "\n"
      end
      str
    end
  end
end
