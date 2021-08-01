module Realm
  class Being
    # TODO: Quest Objekt type
    # TODO: Avatar log
    # TODO: OLE for Quests
    # TODO: Accept, decline, abandon a quest
    # TODO: List quests (w/ filters, incl. available and completed)

    ###########################################################################
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
