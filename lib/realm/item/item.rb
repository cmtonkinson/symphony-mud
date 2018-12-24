module Realm
  class Item < Objekt
    attr_accessor :level, :type, :wearloc

    TYPES = %i(armor clothing container food furniture jewelry key other weapon)
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
      @level   = 1
      @type    = :other
      @wearloc = nil
    }

    ###########################################################################
    on_hashify -> {
      hash = {}

      hash[:level] = @level
      hash[:type] = @type
      hash[:wearloc] = @wearloc if wearable?

      hash
    }

    ###########################################################################
    on_dehashify -> hash {
      hash

      @level   = hash[:level]
      @type    = hash[:type]
      @wearloc = hash[:wearloc] if wearable?
    }

    ###########################################################################
    def wearable?
      return %i(armor clothing jewelry weapon).include? type
    end

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
