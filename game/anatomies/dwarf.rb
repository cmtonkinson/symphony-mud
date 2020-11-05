module Talent
  class Dwarf < Anatomy
    def initialize(being)
      super
      @name = 'dwarf'

      add_slot self, :head, 'head'
      add_slot self, :ear, 'left ear'
      add_slot self, :ear, 'right ear'
      add_slot self, :face, 'face'
      add_slot self, :neck, 'neck'
      add_slot self, :neck, 'neck'
      add_slot self, :shoulders, 'shoulders'
      add_slot self, :arms, 'arms'
      add_slot self, :torso, 'torso'
      add_slot self, :forearm, 'left forearm'
      add_slot self, :forearm, 'right forearm'
      add_slot self, :wrist, 'left wrist'
      add_slot self, :wrist, 'right wrist'
      add_slot self, :hands, 'hands'
      add_slot self, :hold, 'left hand'
      add_slot self, :hold, 'right hand'
      add_slot self, :finger, 'left finger'
      add_slot self, :finger, 'right finger'
      add_slot self, :waist, 'waist'
      add_slot self, :waist, 'waist'
      add_slot self, :legs, 'legs'
      add_slot self, :knee, 'left knee'
      add_slot self, :knee, 'right knee'
      add_slot self, :shin, 'left shin'
      add_slot self, :shin, 'right shin'
      add_slot self, :ankle, 'left ankle'
      add_slot self, :ankle, 'right ankle'
      add_slot self, :feet, 'feet'

      setup
    end

    def modifiers
      {
        constitution: 2,
        charisma: -1
      }
    end
  end
end
