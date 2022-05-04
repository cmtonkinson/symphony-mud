# The 'information' command at the end of these are just to get the full state
# of the quest into the log.
module Test
  define :create_quest do
    with :admin
    test do
      admin 'commands'
      expect 'qedit'

      admin 'qedit create'
      expect "You're now editing quest"

      admin 'info'
    end
  end

  define :add_task do
    from :create_quest
    with :admin
    test do
      admin 'qedit 1'
      admin 'task add TaskA'
      expect 'added with name TaskA'

      admin 'info'
      expect '1'
      expect 'TaskA'
    end
  end

  define :delete_task do
    from :add_task
    with :admin
    test do
      admin 'qedit 1'
      admin 'task 1 remove'
      expect '1 removed'

      admin 'info'
      n_expect 'TaskA'
    end
  end

  define :set_task_description do
    from :add_task
    with :admin
    test do
      admin 'qedit 1'
      admin 'task 1 description foobar'
      expect 'foobar'
      admin 'info'
    end
  end

  define :set_task_title do
    from :add_task
    with :admin
    test do
      admin 'qedit 1'
      admin 'task 1 title NewName'
      expect 'NewName'
      admin 'info'
    end
  end

  define :set_task_objective do
    from :add_task
    with :admin
    test do
      admin 'qedit 1'
      admin 'task 1 objective visit_room 2'
      expect 'room'
      admin 'info'
    end
  end

  define :set_task_rewards do
    from :add_task
    with :admin
    test do
      admin 'qedit 1'

      %w[silver gold exp].each do |kind|
        admin "task 1 reward #{kind} 10"
        expect "10 #{kind}"
        admin 'info'
        expect "10 #{kind}"

        admin "task 1 reward #{kind} 0"
        expect "0 #{kind}"
        admin 'info'
        n_expect kind
      end

      admin 'info'
    end
  end

  define :set_activation do
    from :create_quest
    with :admin
    test do
      admin 'qedit 1'
      admin 'activation enter_room 2'
      expect 'enter_room -- 2'
      admin 'info'
    end
  end

  define :set_title do
    from :create_quest
    with :admin
    test do
      admin 'qedit 1'
      admin 'title A Test Quest in Jest'
      expect 'A Test Quest in Jest'
      admin 'info'
    end
  end

  define :create_full_quest do
    from :create_quest
    with :admin
    test do
      # create Rooms 2 and 3
      # the quest will activate in room 2 and the task objective is to visit 3
      admin 'redit'
      admin 'dig east'
      admin 'dig north'
      admin 'exit'
      admin 'goto 1'
      admin 'map'

      # create the quest
      admin 'qedit 1'
      admin 'title A New Quest'
      admin 'activation enter_room 2'
      admin 'task add The First Task'
      admin 'task 1 description Go North'
      admin 'task 1 objective visit_room 3'
      admin 'task 1 reward silver 100'
      admin 'task 1 reward gold 100'
      admin 'task 1 reward exp 100'

      # verify
      admin 'info'
      admin 'exit'
      admin 'quest list'
      expect "don't have any quests available"
    end
  end

  define :assign_quest do
    from :create_full_quest
    with :frodo
    test do
      frodo 'east'
      expect 'You have a new quest available'
      expect 'A New Quest'

      frodo 'quest list'
      expect 'A New Quest'
      expect 'available'
      n_expect 'accepted'

      frodo 'quest accept 2'
      expect "Can't find that"

      frodo 'quest accept 1'
      expect 'You have accepted A New Quest'

      frodo 'quest list'
      expect 'accepted'
      n_expect 'available'

      frodo 'west'
    end
  end

  define :dont_double_assign do
    from :assign_quest
    with :frodo
    test do
      frodo 'east'
      n_expect 'You have a new quest available'
    end
  end

  define :complete_first_task do
    from :assign_quest
    with :frodo
    test do
      frodo 'east'
      frodo 'north'
      expect 'complete'
    end
  end

  define :dont_double_complete do
    from :complete_first_task
    with :frodo
    test do
      frodo 'south'
      frodo 'north'
      n_expect 'complete'
    end
  end
end
