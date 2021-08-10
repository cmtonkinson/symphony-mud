module Realm
  class Being
    # TODO: Quest Objekt type
    # TODO: Avatar log
    # TODO: OLE for Quests
    # TODO: Accept, decline, abandon a quest
    # TODO: List quests (w/ filters, incl. available and completed)
    attr_accessor :quests

    after_initialize lambda {
      @quests = []
    }

    on_hashify lambda {
      {
        quests: @quests.map(&:hashify)
      }
    }

    on_dehashify lambda { |hash|
      return unless hash[:quests]

      @quests = []
      hash[:quests].each do |h|
        @quests << Realm::Quest.dehashify(h)
        # TODO: 'link' each Quest to the Being (activate effect triggers, etc)
      end
    }

    task_completed lambda { |task|
      puts "you completed #{task.name}"

      task.rewards.each do |reward|
        reward.to_str_array.each do |str|
          puts "  you receive #{str}"
        end
      end
    }

    task_failed lambda { |task|
      puts "you failed #{task.name}"
    }

    task_updated lambda { |task|
      puts "task #{task.name} updated"
    }

    # Add the given Quest to the players list of quests and notify them that it
    # has become available to them.
    def assign_quest(quest)
      q = quest.clone
      q.rid = next_quest_rid
      q.hero = self
      q.assign!
      @quests << q

      puts "\nYou have a new quest available!"
      puts '  [{C$rid{x] {Y$quest{x', q, q
    end

    # Enable Tasks and Spys for this Quest.
    def accept_quest(quest)
      return unless @quests.include? quest
      return unless quest.assigned?

      quest.accept!
      puts "\nYou have accepted {Y$quest{x. First up:", quest
      quest.tasks.each do |t|
        t.activate self
        puts "\n  {W$task{x", t
      end
    end

    # Forget all about this Quest.
    def abandon_quest(quest)
      return unless @quests.include? quest

      # TODO: ensure there are no further cleanup tasks needed here
      @quests.delete quest
      quest.run_hook :enervate
    end

    # for Quests, RIDs are meant to act like an auto-increment number from 1
    def next_quest_rid
      return 1 if @quests.empty?

      @quests.map(&:rid).max + 1
    end

    def accept_task(task)
      # register callback for objective
      #   - this means the objective needs to be phrased in terms of callbacks
      #   - that make sense to the Being (e.g. `:visit_room, 4332`)
      #
      # register callbacks for parameters
      #   - parameters may invalidate completion of the objective
      #   - e.g. if you enter a fight or go to sleep, perhaps the Task resets/abandons
      #   - also setting a timer could invalidate a Task
    end
  end
end
