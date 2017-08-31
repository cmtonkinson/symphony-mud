module Combat

  class Battle
    include Hooks

    define_hooks :after_group_added, :after_group_deleted

    attr_reader :groups, :bouts, :job

    ###########################################################################
    def initialize
      @groups = Array.new
      @bouts  = Array.new
      @job    = Mechanics::RecurringJob.new 0, self, 2, :new_bout

      $world.schedule @job
    end

    ###########################################################################
    def add_group(group)
      raise ArugmentError.new "Cannot add a #{group.class.name} as a Battle group" unless group.is_a? Combat::Group
      @groups.push group
      group.battle = self
      self.run_hook :after_group_added, group
    end

    ###########################################################################
    def delete_group(group)
      @groups.delete group
      group.battle = nil
      self.run_hook :after_group_deleted, group
    end

    ###########################################################################
    # Initiate a new "bout" or "round" of combat.
    #
    # First verifies that there are remaining eligible combatants. If a Group
    # contains no eligible combatants, it is removed from the Battle. If there
    # are not at least two remaining Groups, combat ends. Keep in mind that
    # the test for an "eligible combatant" includes the criteria of being able
    # to acquire a target - for example if you have two Groups both fighting a
    # third (but not each other) and the third Group wipes, Battle would end
    # because although there are two Groups left, they are not opponents of
    # one another.
    #
    # However, if these conditions ARE met, a new Bout is created whereupon
    # each eligible combatant gets a turn to attack one defender.
    def new_bout
      @groups.each do |group|
        if group.members.none? { |m| m.acquire_target }
          Symphony.logger.trace "Group removed from Battle: no targets"
          delete_group group
        end
      end

      if @groups.empty?
        Symphony.logger.trace "Battle over: no opposing Groups"
        return
      end

      @bouts.push Bout.new(self)
    end

    ###########################################################################
    after_group_added -> (group) {
    }

    ###########################################################################
    after_group_deleted -> (group) {
      # If less than two groups, end the battle
      if @groups.size < 2
        # Remove the remaining Group (if there is one).
        @groups.each { |b| delete_group b }
        # Unschedule the next Bout.
        @job.kill
      end
    }

  end

end

