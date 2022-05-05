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
        q = Realm::Quest.dehashify(h)
        q.hero = self
        @quests << q
        q.activate if q.accepted?
      end
    }

    task_completed lambda { |task|
      Symphony.logger.debug "#{task.hero.fqid_str} completed #{task.fqid_str}"
      puts "you completed #{task.name}"
      task.reward.to_str_array.each do |str|
        puts "  you receive #{str}"
      end
    }

    task_failed lambda { |task|
      Symphony.logger.debug "#{task.hero.fqid_str} failed #{task.fqid_str}"
      puts "you failed #{task.name}"
    }

    task_updated lambda { |task|
      Symphony.logger.trace "#{task.hero.fqid_str} updated #{task.fqid_str}"
      puts "task #{task.name} updated"
    }

    quest_complete lambda { |quest|
      Symphony.logger.debug "#{quest.hero.fqid_str} completed #{quest.fqid_str}"
      puts "you completed quest #{quest.name}"
    }

    quest_failed lambda { |quest|
      Symphony.logger.debug "#{quest.hero.fqid_str} failed #{quest.fqid_str}"
      puts "you failed quest #{quest.name}"
    }

    # Add the given Quest to the players list of quests and notify them that it
    # has become available to them.
    def assign_quest(quest)
      q = quest.clone
      q.rid = next_quest_rid
      q.hero = self
      q.assign!
      @quests << q
      Symphony.logger.trace "#{q.fqid_str} (from #{quest}) assigned to #{fqid_str}"
      puts "\nYou have a new quest available!"
      puts '  [{C$rid{x] {Y$quest{x', q, q
    end

    # Enable Tasks and Spys for this Quest.
    def accept_quest(quest)
      return unless @quests.include? quest
      return unless quest.assigned?

      quest.accept!
      Symphony.logger.debug "#{fqid_str} accepted by #{quest.fqid_str}"
      puts "\nYou have accepted {Y$quest{x. First up:", quest
      quest.tasks.each { |t| puts "\n  {W$task{x", t }
    end

    # Forget all about this Quest.
    def abandon_quest(quest)
      return unless @quests.include? quest

      # TODO: ensure there are no further cleanup tasks needed here
      Symphony.logger.debug "#{quest.fqid_str} abandoned by #{fqid_str}"
      @quests.delete quest
      quest.run_hook :enervate
    end

    # for Quests, RIDs are meant to act like an auto-increment number from 1
    def next_quest_rid
      @quests.empty? ? 1 : (@quests.map(&:rid).max + 1)
    end
  end
end
