module Konst
  AVATAR_PREFS = %w[color numeric].freeze

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
end
