module Talent
  class Klass
    include Talent::Enabler

    attr_reader :health_rate
    attr_reader :mana_rate

    ###########################################################################
    def initialize
      @name        = ''
      @health_rate = 0
      @mana_rate   = 0
      @abilities   = Array.new
    end

  end
end
