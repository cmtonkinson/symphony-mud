module Konst
  AVATAR_PREFS = %w[color numeric].freeze

  BEING_RESIST_NEUTRAL = 100
  BEING_RESIST_MIN     = -100
  BEING_RESIST_MAX     = 200

  DAMAGE_TYPES = %i[bash slash pierce exotic].freeze
  DAMAGE_MIN_STAMINA_LOSS = 1
  DAMAGE_MAX_STAMINA_LOSS = 20

  ELEMENTS = %i[air earth fire ice water arcane divine fell].freeze

  ITEM_ATTRS = %i[level type wearable slot max_items load_factor health mana
                  max_occupants opens_type opens_rid weapon_type damage_type
                  element_type element_ratio hitroll damroll].freeze
  ITEM_TYPES = %i[armor clothing container food furniture jewelry key other
                  weapon].freeze
  ITEM_ELEMENT_MIN = 0
  ITEM_ELEMENT_MAX = 100

  # Triggers available for configuration in Objectives and Parameters.
  SPY_TRIGGERS = %i[
    consume_item
    die
    drop_item_at
    enter_combat
    explore_zone
    give_item_to
    kill_npc
    obtain_item
    obtain_item
    put_item_in
    say_at
    say_to
    visit_room
    wear_item
  ].freeze

  # Parameters have different types that map into triggers above. Note that
  # only triggers defined above in SPY_TRIGGERS are valid keys for this Hash.
  SPY_PARAMETERS = {
    no_dying: :die,
    no_fighting: :enter_combat,
    time_limit: nil
  }.freeze
end
