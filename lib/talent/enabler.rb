module Talent
  module Enabler
    def name
      @name ||= ''
    end

    def abilities
      @abilities ||= []
    end

    def add_ability(ability)
      abilities.push ability
    end

    def add_prerequisite(dependent_class, independent_class)
      # Find both Ability objects.
      dependent = @abilities.find { |ability| ability.instance_of? dependent_class }
      Util.argerr "cannot find dependent Ability by class '#{dependent_class}'" if dependent.nil?

      independent = @abilities.find { |ability| ability.instance_of? independent_class }
      Util.argerr "cannot find independent Ability by class '#{independent_class}'" if independent.nil?

      independent.add_prerequisite dependent
    end

    # Roots are Abilities without other Abilities as prerequisites.
    def root_abilities
      abilities.select { |a| a.prerequisites.empty? }
    end
  end
end
