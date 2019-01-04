require_relative '../realm/objekt'

module Combat
  class Battle < Realm::Objekt
    define_hooks :after_group_added, :after_group_deleted

    attr_reader :groups, :bouts, :job

    ###########################################################################
    after_initialize -> {
      @groups = Array.new
      @bouts  = Array.new
      @job    = Mechanics::RecurringJob.new 0, self, 2, :new_bout

      $world.schedule @job
    }

    ###########################################################################
    def add_group(group)
      raise ArugmentError.new "Cannot add a #{group.class.name} as a Battle group" unless group.is_a? Combat::Group
      return if @groups.include? group

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
    # are not at least two remaining Groups, combat ends. If there are at least
    # two opposing Groups a new Bout is created whereupon each eligible
    # combatant gets a turn to attack one defender.
    def new_bout
      @groups.each { |g| delete_group g unless g.acquire_targets }

      if @groups.empty?
        cease
      else
        bout = Bout.new(self)
        @bouts.push bout
        bout.start
      end
    end

    ###########################################################################
    def cease
      Symphony.logger.debug 'Battle ceasing'
      run_hook :enervate
    end

    ###########################################################################
    enervate -> {
      dispose
    }

    ###########################################################################
    after_group_added ->(group) {}

    ###########################################################################
    after_group_deleted ->(_group) {
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
