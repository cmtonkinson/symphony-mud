module Talent
  class Human < Anatomy
    def initialize(being)
      super
      @name = 'human'

      add_slot :head, 'head'
      add_slot :ear, 'left ear'
      add_slot :ear, 'right ear'
      add_slot :face, 'face'
      add_slot :neck, 'neck'
      add_slot :neck, 'neck'
      add_slot :shoulders, 'shoulders'
      add_slot :arms, 'arms'
      add_slot :torso, 'torso'
      add_slot :forearm, 'left forearm'
      add_slot :forearm, 'right forearm'
      add_slot :wrist, 'left wrist'
      add_slot :wrist, 'right wrist'
      add_slot :hands, 'hands'
      add_slot :hold, 'left hand'
      add_slot :hold, 'right hand'
      add_slot :finger, 'left finger'
      add_slot :finger, 'right finger'
      add_slot :waist, 'waist'
      add_slot :waist, 'waist'
      add_slot :legs, 'legs'
      add_slot :knee, 'left knee'
      add_slot :knee, 'right knee'
      add_slot :shin, 'left shin'
      add_slot :shin, 'right shin'
      add_slot :ankle, 'left ankle'
      add_slot :ankle, 'right ankle'
      add_slot :feet, 'feet'

      setup
    end

    def modifiers
      {
        intelligence: 2,
        luck: -1
      }
    end
  end
end
