module Realm
  class Objekt
    attr_reader :jobs

    after_initialize lambda {
      # Every time a Job is created, if it is created for an Objekt, then the
      # Objekt tracks the Job. This is primarily so that when an Objekt is
      # disposed of while it has pending Jobs, they can be killed.
      @jobs = Set.new
    }

    before_disposal lambda {
      # Mark each Job registered to this Objekt as killed.
      @jobs.each(&:kill)
      @jobs.clear
    }
  end
end
