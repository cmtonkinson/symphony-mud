module Command
  ###############################################################################
  define :iedit do
    level Realm::Being::DEMIGOD
    usage 'create'
    usage({ rid: :int })
    usage :target
    exec do
      if create == 'create'
        item = Logistics::Builder.create_item room.zone
        being.cmd "iedit #{item.rid}"
      elsif rid.is_a? Integer
        zone = room.zone
        item = zone.find_by_rid Realm::Item, rid
        being.puts t 'err_nothing' and next if item.nil?

        being.iedit = item
        being.user.operators.push Operator.new being.user, :edit_item
        being.print t 'edit'
        being.puts I18n.t('objekt.identify', being.iedit.fqid)
        being.puts being.iedit.summary
      else
        item = being.items.locate_first target
        being.puts t 'err_not_holding' and next if item.nil?

        being.iedit = item
        being.user.operators.push Operator.new being.user, :edit_item
        being.print t 'edit'
        being.puts I18n.t('objekt.identify', being.iedit.fqid)
        being.puts being.iedit.summary
      end
    end
  end

  ###############################################################################
  define :items do
    level Realm::Being::DEMIGOD
    usage
    usage({ zone_rid: :int })
    exec do
      zone = zone_rid.nil? ? being.room.zone : $world.zones.values.find { |z| z.rid == zone_rid }
      being.puts t 'err_nowhere' and next if zone.nil?

      being.puts t('title'), zone
      zone.items.each do |_, item|
        being.puts t('line'), item, item
      end
    end
  end

  ###############################################################################
  define :information, :edit_item do
    level Realm::Being::DEMIGOD
    usage
    exec do
      being.puts being.iedit.summary
    end
  end

  ###############################################################################
  define :exit, :edit_item do
    level Realm::Being::DEMIGOD
    usage
    exec do
      being.puts t 'actor'
      being.iedit = nil
      being.user.operators.pop
    end
  end

  modifiable_properties = (%i[health mana] +
                           Realm::Being::FITNESS +
                           Realm::Being::DEFENSE +
                           Realm::Being::OFFENSE
                          ).map(&:to_s)
  ###############################################################################
  define :modify, :edit_item do
    level Realm::Being::DEMIGOD
    usage({ property: modifiable_properties }, { amount: :int })
    exec do
      being.puts t('err_zero') and next if amount == 0

      being.iedit.modifiers[property.to_sym] = amount
      being.puts t('modified'), property, amount
    end
  end

  ###############################################################################
  define :unmodify, :edit_item do
    level Realm::Being::DEMIGOD
    usage({ property: modifiable_properties })
    exec do
      being.iedit.modifiers.delete property.to_sym
      being.puts t('unmodified'), property, amount
    end
  end

  ###############################################################################
  define :wearable, :edit_item do
    level Realm::Being::DEMIGOD
    usage 'none'
    usage({ slot: :string })
    exec do
      if none == 'none'
        being.iedit.wearable = false
        being.puts t('none')
      else
        # TODO: - validate that the slot type actually exists (requires iterating
        # all known Anatomies)
        being.iedit.wearable = true
        being.iedit.slot = slot.to_sym
        being.puts t('set'), slot
      end
    end
  end

  ###############################################################################
  is_positive = ->(x) { return (x > 0) }
  is_word     = ->(x) { return !x.match(/\Aa-z+\z/i) }
  in_types    = ->(x) { return Realm::Item::TYPES.include?(x.to_sym) }

  [
    [:name, :rest, nil, ->(x) { !x.empty? }, 'cannot be blank', nil],
    [:level, :int, nil, is_positive, 'must be 1 or higher', nil],
    [:type, :string, nil, in_types, "must be one of: #{Realm::Item::TYPES.join(', ')}", ->(x) { return x.to_sym }],
    [:slot, :string, nil, is_word, 'must be a word', ->(x) { return x.to_sym }],
    [:max_items, :int, :container, is_positive, 'must be 1 or higher', nil],
    [:load_factor, :int, :container, is_positive, 'must be 1 or higher', nil],
    [:health, :int, :food, is_positive, 'must be 1 or higher', nil],
    [:mana, :int, :food, is_positive, 'must be 1 or higher', nil],
    [:max_occupants, :int, :furniture, is_positive, 'must be 1 or higher', nil],
    [:opens_type, :string, :key, ->(x) { return %w[room item].include?(x) }, 'must be either room or item', nil],
    [:opens_rid, :int, :key, is_positive, 'must be 1 or higher', nil],
    [:weapon_type, :string, :weapon, is_word, 'must be a word', nil],
    [:damage_type, :string, :weapon, is_word, 'must be a word', nil],
    [:hitroll, :int, :weapon, is_positive, 'must be 1 or higher', nil],
    [:damroll, :int, :weapon, is_positive, 'must be 1 or higher', nil]
  ].each do |definition|
    name, type, subtype, validation, hint, conversion = *definition
    define name, :edit_item do
      level Realm::Being::DEMIGOD
      usage({ value: type })
      exec do
        if !subtype.nil? && subtype != being.iedit.type
          being.puts "That property only applies to items that are a #{subtype}." and next
        end
        being.puts "Failed to update #{name} - new value #{hint}." and next unless validation.call(value)

        being.iedit.send "#{name}=", (conversion.nil? ? value : conversion.call(value))
        new_value = being.iedit.send name.to_s
        being.puts "Item #{name} changed to #{new_value}"
      end
    end
  end
end
