module Talent
  class Klass
    include Talent::Enabler

    attr_reader :being
    attr_reader :health_rate
    attr_reader :mana_rate

    def initialize(being)
      @name        = ''
      @being       = being
      @health_rate = 0
      @mana_rate   = 0
    end

    def self.klasses
      ObjectSpace.each_object(::Class).select { |c| c < self }
    end

    def setup
      being.klass = self
      being.add_enabler self
      being.modify modifiers if respond_to? :modifiers
    end
  end
end
