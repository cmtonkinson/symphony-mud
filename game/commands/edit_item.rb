module Command

###############################################################################
define :iedit do
###############################################################################
  level Realm::Being::DEMIGOD
  usage 'create'
  usage({rid: :int})
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
    end
  end
end

###############################################################################
define :items do
###############################################################################
  level Realm::Being::DEMIGOD
  usage
  usage({zone_rid: :int})
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
###############################################################################
  level Realm::Being::DEMIGOD
  usage
  exec do
    being.puts being.iedit.summary
  end
end

###############################################################################
define :exit, :edit_item do
###############################################################################
  level Realm::Being::DEMIGOD
  usage
  exec do
    being.puts t 'actor'
    being.iedit = nil
    being.user.operators.pop
  end
end

###############################################################################
define :level, :edit_item do
###############################################################################
  level Realm::Being::DEMIGOD
  usage({new_level: :int})
  exec do
    being.iedit.level = new_level
    being.puts t('actor'), being.iedit.level
  end
end

###############################################################################
define :name, :edit_item do
###############################################################################
  level Realm::Being::DEMIGOD
  usage :rest
  exec do
    being.iedit.name = rest
    being.puts t('actor'), being.iedit.name
  end
end

end
