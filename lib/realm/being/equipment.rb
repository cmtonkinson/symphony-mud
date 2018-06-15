module Realm
  class Being

    WEARLOCS = {
      head:       "head",
      ear_l:      "left ear",
      ear_r:      "right ear",
      face:       "face",
      neck:       "neck",
      shoulders:  "shoulders",
      arms:       "arms",
      torso:      "torso",
      forearm_l:  "left forearm",
      forearm_r:  "right forearm",
      wrist_l:    "left wrist",
      wrist_r:    "right wrist",
      hands:      "hands",
      hold_l:     "left hand",
      hold_r:     "right hand",
      finger_l:   "left finger",
      finger_r:   "right finger",
      waist_1:    "waist",
      waist_2:    "waist",
      legs:       "legs",
      knee_l:     "left knee",
      knee_r:     "right knee",
      shin_l:     "left shin",
      shin_r:     "right shin",
      ankle_l:    "left ankle",
      ankle_r:    "right ankle",
      feet:       "feet",
    }
    # attr_accessor

    ###########################################################################
    after_initialize -> {
    }

    ###########################################################################
    on_hashify -> {
    }

    ###########################################################################
    on_dehashify -> hash {
    }

  end
end
