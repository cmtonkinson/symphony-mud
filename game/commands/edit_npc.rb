module Command

###############################################################################
define :nedit do
###############################################################################
  level Realm::Being::DEMIGOD
  usage 'create'
  usage({rid: :int})
  exec do
    if create == 'create'
      npc = Logistics::Builder.create_npc room.zone
      being.cmd "nedit #{npc.rid}"
    elsif rid.is_a? Integer
      zone = room.zone
      npc = zone.find_by_rid Realm::NPC, rid
      being.puts t 'err_nothing' and next if npc.nil?

      being.nedit = npc
      being.user.operators.push Operator.new being.user, :edit_npc
      being.print t 'edit'
      being.puts I18n.t('objekt.identify', being.nedit.fqid)
      being.puts being.nedit.summary
    end
  end
end

###############################################################################
define :npcs do
###############################################################################
  level Realm::Being::DEMIGOD
  usage
  usage({zone_rid: :int})
  exec do
    zone = zone_rid.nil? ? being.room.zone : $world.zones.values.find { |z| z.rid == zone_rid }
    being.puts t 'err_nowhere' and next if zone.nil?

    being.puts t('title'), zone
    zone.npcs.each do |_, npc|
      being.puts t('line'), npc, npc
    end
  end
end

###############################################################################
define :information, :edit_npc do
###############################################################################
  level Realm::Being::DEMIGOD
  usage
  exec do
    being.puts being.nedit.summary
  end
end

###############################################################################
define :exit, :edit_npc do
###############################################################################
  level Realm::Being::DEMIGOD
  usage
  exec do
    being.puts t 'actor'
    being.nedit = nil
    being.user.operators.pop
  end
end

###############################################################################
define :level, :edit_npc do
###############################################################################
  level Realm::Being::DEMIGOD
  usage({new_level: :int})
  exec do
    being.nedit.level = new_level
    being.puts t('actor'), being.nedit.level
  end
end

###############################################################################
define :name, :edit_npc do
###############################################################################
  level Realm::Being::DEMIGOD
  usage :rest
  exec do
    being.nedit.name = rest
    being.puts t('actor'), being.nedit.name
  end
end

end
