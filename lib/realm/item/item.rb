module Realm
  class Item < Objekt
    TYPES = %i(armor clothing container food furniture jewelry key other weapon)

    ATTRS = %i(level type wearable slot
               max_items load_factor
               health mana
               max_occupants
               opens_type opens_rid
               weapon_type damage_type hitroll damroll)

    ATTRS.each { |name| attr_accessor name }

    attr_accessor :modifiers

    ###########################################################################
    def initialize(id = nil, props = {})
      super
    end

    ###########################################################################
    after_initialize -> {
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
      @hitroll       = 0
      @damroll       = 0
      @modifiers     = Hash.new
    }

    ###########################################################################
    on_hashify -> {
      hash = {}
      ATTRS.each { |name| hash[name] = send name }
      hash[:modifiers] = @modifiers
      hash
    }

    ###########################################################################
    on_dehashify ->(hash) {
      ATTRS.each { |name| instance_variable_set "@#{name}", hash[name] }
      @modifiers = hash[:modifiers]
    }

    ###########################################################################
    on_deep_copy ->(original) {
      ATTRS.each { |name| instance_variable_set "@#{name}", original.send(name) }
      @modifiers = Marshal.load(Marshal.dump(original.modifiers))
    }

    ###########################################################################
    enervate -> {
      dispose
    }

    ###########################################################################
    def _interpret_name
      '$item'
    end

    ###########################################################################
    def summary
      str = $templates.item_summary % [
        rid, name,
        id,
        level, type,
        %w[].sort.join(', '),
        *(wearable ? ['yes', slot.to_s] : ['no', 'N/A']),
      ]
      if type == :weapon
        str << $templates.item_weapon_summary % [
          weapon_type, damage_type, hitroll, damroll
        ]
      end
      if wearable && !@modifiers.empty?
        str << "{xModfiers:\n"
        @modifiers.each do |name, amount|
          str << "{x %-15s %s%d{x\n" % [name.to_s, amount > 0 ? '{G+' : '{R', amount]
        end
      end
      str
    end
  end
end
