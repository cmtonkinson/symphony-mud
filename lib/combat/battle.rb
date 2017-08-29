module Combat

  class Battle
    include Hooks

    define_hooks :after_belligerent_added, :after_belligerent_deleted

    attr_reader :belligerents, :bouts, :job

    ###########################################################################
    def initialize
      @belligerents = Set.new
      @bouts        = []
      @job          = Mechanics::RecurringJob.new 0, self, 2, :new_bout 

      $world.schedule @job
    end

    ###########################################################################
    def add_belligerent(group)
      @belligerents.add group
      group.battle = self
      self.run_hook :after_belligerent_added, group
    end

    ###########################################################################
    def delete_belligerent(group)
      @belligerents.delete group
      group.battle = nil
      self.run_hook :after_belligerent_deleted, group
    end

    ###########################################################################
    def new_bout
      @bouts << Bout.new(self)
    end

    ###########################################################################
    after_belligerent_added -> (belligerent) {
    }

    ###########################################################################
    after_belligerent_deleted -> (belligerent) {
      if @belligerents.size < 2
        # Remove the remaining Belligerent (if there is one).
        @belligerents.each { |b| delete_belligerent b }
        # Unschedule the next Bout.
        @job.kill
      end
    }

  end

end

