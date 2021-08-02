module Realm
  class Objekt
    # These hooks are used for Task management. Eventually most Tasks will
    # probably either be completed or failed (and will run the corresponding
    # hook) exactly once, but in a general sense, you really can't make any
    # assumptions about if, when, or how often Tasks will run the updated
    # hook due to the fact that it is specific to the implemntation of the
    # individual Spy types.
    #
    # In all cases, these hooks are passed a single parameter, which is the
    # Task object in question.
    define_hook :task_completed
    define_hook :task_failed
    define_hook :task_updated
  end
end
