module Talent
  module Enabler

    ###########################################################################
    def name
      @name ||= ''
    end

    ###########################################################################
    def abilities
      @abilities ||= Array.new
    end

    ###########################################################################
    def add_ability(ability)
      abilities.push ability
    end

    ###########################################################################
    def add_dependency(ability, prereq)
      ability.add_prerequisite prereq
    end

    ###########################################################################
    # Roots are Abilities without other Abilities as prerequisites.
    def root_abilities
      abilities.select { |a| a.prerequisites.empty? }
    end
  end
end
