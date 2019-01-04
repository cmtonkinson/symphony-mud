module Realm
  class NPC

    ###########################################################################
    after_death -> {
      group.disband if group && group.leader == self
      run_hook :enervate
    }

  end
end
