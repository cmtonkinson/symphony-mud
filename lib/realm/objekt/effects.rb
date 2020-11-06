module Realm
  class Objekt
    attr_reader :effects

    ###########################################################################
    after_initialize lambda {
      @effects = Set.new
    }

    ###########################################################################
    enervate lambda {
      effects.first.expire until effects.empty?
    }

    ###########################################################################
    on_hashify lambda {
      hash = {}
      hash[:effects] = effects.map(&:hashify)
      hash
    }

    ###########################################################################
    on_dehashify lambda { |hash|
      return unless hash.key? :effects

      hash[:effects].each do |h|
        e = Realm::Effect.dehashify(h)
        e.target = self
        effects.add e
      end
    }
  end
end
