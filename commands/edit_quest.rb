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
        quest = room.zone.find_by_rid Realm::Quest, rid
        being.puts t 'err_nothing' and next if quest.nil?

        being.qedit = quest
        being.user.operators.push Operator.new being.user, :edit_quest
        being.print t 'edit'
        being.puts I18n.t('objekt.identify', **being.qedit.fqid)
        being.puts being.qedit.summary
      end
    end
  end

  define :information, :edit_quest do
    level Realm::Being::DEMIGOD
    usage
    exec do
      being.puts being.qedit.summary
    end
  end

  define :task, :edit_quest do
    level Realm::Being::DEMIGOD
    usage('add', :rest)
    usage({ rid: :int }, 'delete')
    usage({ rid: :int }, 'description', :rest)
    usage({ rid: :int }, 'objective', { type: :string })
    usage({ rid: :int }, 'parameter', 'add', { type: :string }, :rest)
    usage({ rid: :int }, 'parameter', 'delete', { type: :string })
    usage({ rid: :int }, 'parent', { parent_rid: :int })
    usage({ rid: :int }, 'reward', 'add', { type: :string }, :rest)
    usage({ rid: :int }, 'reward', 'delete', { type: :string })
    usage({ rid: :int }, 'title', :rest)
    exec do
      ####################
      # add
      if add == 'add'
        task = Realm::Task.new nil, rid: being.qedit.next_task_rid, name: rest
        being.qedit.tasks << task
        being.puts t('add'), task.rid, task.name

      elsif rid.is_a? Integer
        task = being.qedit.task_by_rid rid
        being.puts t('err_nothing') and next if task.nil?

        ####################
        # delete
        if delete == 'delete'
          parent = task.parent
          children = task.children

          if parent
            # If the task is a child, then we need to connect its parent
            # with all its children (effectively plucking it out of the tree).
            parent.children.delete task
            children.each do |c|
              parent.children << c
              c.parent = parent
            end
          else
            # If the task was top-level in the quest, then it's children
            # now become top-level tasks.
            being.qedit.tasks.delete task
            children.each do |c|
              being.qedit.tasks << c
              c.parent = nil
            end
          end

          being.puts t('deleted'), task.rid

        ####################
        # description
        elsif description == 'description'
          task.description = rest
          being.puts t('description'), task.rid, task.description

        ####################
        # title
        elsif title == 'title'
          task.title = rest
          being.puts t('title'), task.rid, task.title

        ####################
        # error
        else
          Symphony.logger.error "qedit task: bad action (#{action})"
        end
      end
    end
  end
end
