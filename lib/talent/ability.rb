module Talent
  class Ability

    attr_reader :name
    attr_reader :level, :trains, :difficulty, :stamina, :mana, :successRate
    attr_reader :prerequisites

    ###########################################################################
    def initialize(level, trains, difficulty, stamina, mana, successRate)
      @name          = nil
      @level         = level
      @trains        = trains
      @difficulty    = difficulty
      @stamina       = stamina
      @mana          = mana
      @successRate   = successRate
      @prerequisites = Array.new
    end

    ###########################################################################
    def add_prerequisite(prereq)
      @prerequisites.push prereq
    end

    ###########################################################################
    def exec
      raise StandardError.new "Talent::Ability#exec not implemented"
    end

  end
end

