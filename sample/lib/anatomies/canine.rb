module Talent
  class Canine < Anatomy
    NAME = 'Canine'.freeze

    def initialize(being)
      super
      @name = NAME

      add_slot :head, 'head'
      add_slot :neck, 'neck'
      add_slot :neck, 'neck'
      add_slot :neck, 'neck'
      add_slot :torso, 'torso'

      setup
    end
  end
end
