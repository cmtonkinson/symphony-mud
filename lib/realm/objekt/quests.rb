module Realm
  class Objekt
    # See Objeck/Tasks for info
    define_hook :quest_complete
    define_hook :quest_failed
    define_hook :quest_updated
  end
end
