module Talent
  class Ability

    attr_reader :type, :category, :name
    attr_reader :level, :trains, :difficulty, :stamina, :mana, :successRate
    attr_reader :prerequisites
    attr_reader :level, :trains, :difficulty, :stamina, :mana, :successRate

    ###########################################################################
    def initialize(level, trains, difficulty, stamina, mana, successRate)
      @type          = nil # active or passive
      @category      = nil # skill, spell, craft, etc.
      @name          = nil
      @prerequisites = Array.new

      @level         = level
      @trains        = trains
      @difficulty    = difficulty
      @stamina       = stamina
      @mana          = mana
      @successRate   = successRate
    end

    ###########################################################################
    def add_prerequisite(prereq)
      @prerequisites.push prereq
    end

    ###########################################################################
    def category_s
      category.to_s.gsub /_/, ' '
    end

    ###########################################################################
    def name_s
      name.to_s.gsub /_/, ' '
    end

    ###########################################################################
    def added_to
      raise StandardError.new 'Talent::Ability#added_to not redefined'
    end

    ###########################################################################
    def invoke(*args)
      will_run, error = precheck
      return error unless will_run
      _invoke *args
    end

    ###########################################################################
    def precheck
      return true, nil
    end

    ###########################################################################
    def exec
      raise StandardError.new 'Talent::Ability#exec not implemented'
    end
  end
end
