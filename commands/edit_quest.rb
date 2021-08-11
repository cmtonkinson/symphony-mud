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

  define :activation, :edit_quest do
    level Realm::Being::DEMIGOD
    usage('enter_room', { room_id: :int })
    exec do
      a = Realm::Activation.new nil, { quest: being.qedit, trigger: :enter_room, params: room_id }
      being.qedit.activation = a
      being.puts t('actor'), being.qedit.activation.trigger, being.qedit.activation.params
    end
  end

  define :exit, :edit_quest do
    level Realm::Being::DEMIGOD
    usage
    exec do
      being.puts t('actor')
      being.qedit = false
      being.user.operators.pop
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
    usage({ rid: :int }, 'description', :rest)
    usage({ rid: :int }, 'objective', 'visit_room', room_id: :int)
    usage({ rid: :int }, 'parameter', 'add', { type: :string }, :rest)
    usage({ rid: :int }, 'parameter', 'remove', { type: :string })
    usage({ rid: :int }, 'parent', { parent_rid: :int })
    usage({ rid: :int }, 'remove')
    usage({ rid: :int }, 'reward', 'item', { action: %w[add remove] }, :item_id)
    usage({ rid: :int }, 'reward', { kind: %w[silver gold exp] }, { amount: :int })
    usage({ rid: :int }, 'title', :rest)
    # rubocop:disable Metrics/BlockNesting
    exec do
      ####################
      # add
      if add
        task = Realm::Task.new nil, rid: being.qedit.next_task_rid, name: rest
        being.qedit.tasks << task
        being.puts t('add'), task.rid, task.name

      elsif rid.is_a? Integer
        task = being.qedit.task_by_rid rid
        being.puts t('err_nothing') and next if task.nil?

        ####################
        # description
        if description
          task.description = rest
          being.puts t('description'), task.rid, task.description

        ####################
        # objective
        elsif objective
          if visit_room
            task.set_objective :visit_room, room_id
            being.puts t('visit_room'), task.rid, room_id
          end

        ####################
        # remove
        elsif remove
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

          being.puts t('removed'), task.rid

        ####################
        # reward
        elsif reward
          if kind
            task.change_reward_value kind, amount
            being.puts t('reward_amount'), task.rid, amount, kind
          elsif item
            case action
            when 'add'
              task.add_reward_item item_id
              being.puts t('reward_item_add'), task.rid, item_id
            when 'remove'
              task.remove_reward_item item_id
              being.puts t('reward_item_remove'), task.rid, item_id
            else Symphony.logger.error "qedit task reward: bad action (#{action})"
            end
          end

        ####################
        # title
        elsif title
          task.title = rest
          being.puts t('title'), task.rid, task.title

        ####################
        # error
        else
          Symphony.logger.error 'qedit task: bad subcommand'
        end
      end
    end
    # rubocop:enable Metrics/BlockNesting
  end

  define :title, :edit_quest do
    level Realm::Being::DEMIGOD
    usage(:rest)
    exec do
      being.qedit.name = rest
      being.puts t('actor'), being.qedit, being.qedit.name
    end
  end
end
