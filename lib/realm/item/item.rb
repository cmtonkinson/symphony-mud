module Realm
  class Item < Objekt

    attr_accessor :level

# level
# short
# long
# type
#   armor (wearable)
#   clothing (wearable)
#   container
#   food
#   furniture
#   jewelry (wearable)
#   key
#   trash
#   weapon (wearable)

    ###########################################################################
    def initialize(id = nil, props = {})
      super
    end

    ###########################################################################
    after_initialize -> {
      @level = 1
    }

    ###########################################################################
    on_hashify -> {
      {
        level: @level
      }
    }

    ###########################################################################
    on_dehashify -> hash {
      hash

      @level = hash[:level]
    }

    ###########################################################################
    def _interpret_name
      '$item'
    end

    ###########################################################################
    def summary
      $templates.item_summary % [
        rid, name,
        id,
        level,
        %w[].sort.join(', '),
      ]
    end

  end
end
