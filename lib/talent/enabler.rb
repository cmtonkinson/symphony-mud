module Talent
  module Enabler

    attr_reader :name
    attr_reader :abilities

    #############################################################################
    def add_ability(ability)
      @abilities.push ability
    end

    #############################################################################
    def add_dependency(ability, prereq)
      ability.add_prerequisite prereq
    end

    #############################################################################
    # Roots are Abilities without other Abilities as prerequisites.
    def root_abilities
      @abilities.select { |a| a.prerequisites.empty? }
    end

  end
end

