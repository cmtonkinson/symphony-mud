module Command
  define :qedit do
    level Realm::Being::DEMIGOD
    usage 'create'
    usage({ rid: :int })
    exec do
      if create == 'create'
        quest = Logistics::Builder.create_quest room.zone
        being.cmd "qedit #{quest.rid}"
      elsif rid.is_a? Integer
        quest = zone.find_by_rid Realm::Quest, rid
        being.puts t 'err_nothing' and next if quest.nil?

        being.qedit = quest
        being.user.operators.push Operator.new being.user, :edit_quest
        being.print t 'edit'
        being.puts I18n.t('objekt.identify', **being.qedit.fqid)
        being.puts being.qedit.summary
      end
    end

    define :quests do
    end

    define :name, :edit_quest do
    end
  end
end
