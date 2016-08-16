module Command

setup = -> {
  group :edit_item
  level Realm::Being::DEMIGOD
}

precheck = -> {

}

###############################################################################
define :iedit do
###############################################################################
  level Realm::Being::DEMIGOD
  usage "create"
  usage({rid: :int})
  exec do
    if create == "create"
      item = Realm::Builder.create_item room.zone
      being.cmd "iedit #{item.rid}"
    elsif rid.is_a? Integer
      zone = room.zone
      item = zone.find_by_rid Realm::Item, rid
      being.puts t "err_nothing" and next if item.nil?

      being.iedit = item
      being.user.operators.push Operator.new being.user, :edit_item
      being.print t "edit"
      being.puts I18n.t("objekt.identify", being.iedit.identify)
    end
  end
end

###############################################################################
define :information do
###############################################################################
  group :edit_item
  level Realm::Being::DEMIGOD
  usage
  exec do
  end
end

###############################################################################
define :exit do
###############################################################################
  group :edit_item
  level Realm::Being::DEMIGOD
  usage
  exec do
    being.puts t "actor"
    being.iedit = nil
    being.user.operators.pop
  end
end

end
