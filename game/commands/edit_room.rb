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

###############################################################################
define :placement, :edit_room do
###############################################################################
  level Realm::Being::DEMIGOD
  usage "list"
  usage("delete", {rule: :int})
  usage("add", "npc", {npc_rid: :int}, {number: :int}, {max: :int})
  usage("add", "item", {item_rid: :int}, {number: :int}, {max: :int})
  exec do
    if list == "list"
      being.puts t("empty_list") and next if room.placements.empty?
      being.puts t("list_title"), room
      room.placements.values.each { |p| being.puts p.summary }

    elsif delete == "delete"
      placement = room.placements.values.find { |p| p.rid == rule }
      being.puts t("err_nothing") and next if placement.nil?
      placement.remove.from room
      being.puts t("deleted")

    elsif add == "add"
      # NPC or Item?
      if npc == "npc"
        objekt = room.zone.npcs.values.find { |n| n.rid == npc_rid }
        being.puts t("err_no_npc") and next if objekt.nil?
      elsif item == "item"
        objekt = room.zone.items.values.find { |i| i.rid == item_rid }
        being.puts t("err_no_item") and next if objekt.nil?
      end

      placement = Realm::Placement.new(nil, {
        room: room,
        klass: objekt.class.name,
        objekt_id: objekt.id,
        number: number,
        max: max,
      })
      Symphony.logger.debug "%s created %s in %s" % [being, placement, room].map(&:fqid_str)
      placement.add.to room
      being.puts t("new")
      being.cmd "placement list"
    end
  end
end

end

