module Talent
  class Canine < Anatomy
    def initialize(being)
      super
      @name = 'canine'

      add_slot :head, 'head'
      add_slot :neck, 'neck'
      add_slot :neck, 'neck'
      add_slot :neck, 'neck'
      add_slot :torso, 'torso'

      setup
    end
  end
end

