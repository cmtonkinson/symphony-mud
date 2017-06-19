module Command

###############################################################################
define :zedit do
###############################################################################
  level Realm::Being::DEMIGOD
  usage "create"
  usage({zone_rid: :int})
  exec do
    if create == "create"
      new_zone = Logistics::Builder.create_zone
      new_room = Logistics::Builder.create_room new_zone
      being.cmd "goto #{new_room.rid}"
      being.cmd "zedit #{new_zone.rid}"
    elsif zone_rid.is_a? Integer
      zone = $world.find_zone zone_rid
      being.puts t "err_nothing" and next if zone.nil?

      being.zedit = zone
      being.user.operators.push Operator.new being.user, :edit_zone
      being.print t "edit"
      being.puts I18n.t("objekt.identify", being.zedit.fqid)
      being.puts being.zedit.summary
    end
  end
end

###############################################################################
define :information, :edit_zone do
###############################################################################
  level Realm::Being::DEMIGOD
  usage
  exec do
    being.puts being.zedit.summary
  end
end

###############################################################################
define :exit, :edit_zone do
###############################################################################
  level Realm::Being::DEMIGOD
  usage
  exec do
    being.puts t "actor"
    being.zedit = nil
    being.user.operators.pop
  end
end

###############################################################################
define :name, :edit_zone do
###############################################################################
  level Realm::Being::DEMIGOD
  usage :rest
  exec do
    being.zedit.name = rest
    being.puts t("actor"), being.zedit.name
  end
end

end
