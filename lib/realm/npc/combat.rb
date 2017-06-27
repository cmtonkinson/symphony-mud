module Realm

  class NPC

    ###########################################################################
    after_death -> {
      run_hook :enervate
      dispose
    }

  end

end

