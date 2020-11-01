module Realm
  class Objekt
    attr_reader :effects

    after_initialize lambda {
      @effects = Set.new
    }

    enervate lambda {
      until @effects.empty? do
        @effects.first.expire
      end
    }
  end
end
