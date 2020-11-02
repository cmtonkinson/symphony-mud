module Realm
  class Objekt
    attr_reader :effects

    after_initialize lambda {
      @effects = Set.new
    }

    enervate lambda {
      @effects.first.expire until @effects.empty?
    }
  end
end
