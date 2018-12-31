module Realm
  class Being

    attr_accessor :abilities

    define_hooks :before_ability_invocation, :after_ability_invocation

    ###########################################################################
    after_initialize -> {
      @abilities = Array.new
    }

    ###########################################################################
    on_hashify -> {
      hash = Hash.new

      abilities.select(&:learned?).map { |a| hash[a.name] = a.mastery }

      { abilities: hash }
    }

    ###########################################################################
    on_dehashify -> (hash) {
      return unless hash.has_key? :abilities
      hash = hash[:abilities]

      hash.each do |name, mastery|
        get_ability(name).learn mastery
      end
    }

    ###########################################################################
    def add_enabler(enabler)
      @enablers.push enabler
      enabler.abilities.each { |a| add_ability a }
    end

    ###########################################################################
    def add_ability(ability)
      @abilities.push ability
      ability.added_to self
      ability
    end

    ###########################################################################
    def get_ability(name)
      abilities.find { |a| a.name == name.to_sym }
    end

  end
end

