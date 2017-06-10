module Command

###############################################################################
define :redit do
###############################################################################
  level Realm::Being::DEMIGOD
  usage
  usage "create"
  exec do
    if create == "create"
      target_room = Realm::Builder.create_room room.zone
      being.cmd "goto #{target_room.rid}"
      being.cmd "redit"
    else
      being.redit = true
      being.user.operators.push Operator.new being.user, :edit_room
      being.print t "edit"
      being.puts I18n.t("objekt.identify", room.fqid)
      being.puts room.summary
    end
  end
end

###############################################################################
define :information, :edit_room do
###############################################################################
  level Realm::Being::DEMIGOD
  usage
  exec do
    being.puts room.summary
  end
end

###############################################################################
define :exit, :edit_room do
###############################################################################
  level Realm::Being::DEMIGOD
  usage
  exec do
    being.puts t "actor"
    being.redit = false
    being.user.operators.pop
  end
end

###############################################################################
define :name, :edit_room do
###############################################################################
  level Realm::Being::DEMIGOD
  usage :rest
  exec do
    room.name = rest
    being.puts t("actor"), room.name
  end
end

end
