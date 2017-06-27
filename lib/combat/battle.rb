module Combat

  class Battle
    include Hooks

    define_hook :on_death
    define_hook :after_belligerent_deleted

    attr_reader :belligerents, :bouts, :job

    ###########################################################################
    def initialize
      @belligerents = Set.new
      @bouts        = []
      @job          = Mechanics::RecurringJob.new 0, self, 2, :new_bout 

      $world.schedule @job
    end

    ###########################################################################
    def add_belligerent(being)
      @belligerents.add being
      being.circumstance = self
    end

    ###########################################################################
    def delete_belligerent(being)
      @belligerents.delete being
      being.circumstance = nil
      self.run_hook :after_belligerent_deleted
    end

    ###########################################################################
    def opponents_of(being)
      @belligerents.difference([being]).to_a
    end

    ###########################################################################
    def new_bout
      @bouts << Bout.new(self)
    end

    ###########################################################################
    on_death -> (stiff) {
      delete_belligerent stiff
    }

    ###########################################################################
    after_belligerent_deleted -> {
      if @belligerents.size < 2
        # Remove the remaining Belligerent (if there is one).
        @belligerents.each { |b| delete_belligerent b }
        # Unschedule the next Bout.
        @job.kill
      end
    }

  end

end

