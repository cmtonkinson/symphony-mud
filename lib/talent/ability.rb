module Talent
  class Ability

    attr_reader :type, :category, :name
    attr_reader :level, :trains, :difficulty, :stamina, :mana, :successRate
    attr_reader :prerequisites
    attr_reader :able_being

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

      @able_being    = nil
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
    def added_to(being)
      @able_being = being
      _added_to being
    end

    ###########################################################################
    def invoke(*args)
      will_run, error = precheck
      return error unless will_run

      able_being.run_hook :before_ability_invocation, name
      retval = _invoke *args
      able_being.run_hook :after_ability_invocation, name

      retval
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
