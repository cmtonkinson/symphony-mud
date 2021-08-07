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
end
