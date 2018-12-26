module Realm
  class Item < Objekt
    attr_accessor :level, :type, :wearable, :slot

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
      @level    = 1
      @type     = :other
      @wearable = nil
      @slot     = nil
    }

    ###########################################################################
    on_hashify -> {
      hash = {}

      hash[:level]    = @level
      hash[:type]     = @type
      hash[:wearable] = @wearable
      hash[:slot]     = @slot

      hash
    }

    ###########################################################################
    on_dehashify -> hash {
      hash

      @level    = hash[:level]
      @type     = hash[:type]
      @wearable = hash[:wearable]
      @slot     = hash[:slot]
    }

    ###########################################################################
    on_deep_copy -> (original) {
      %w(level type wearable slot).each do |var|
        instance_variable_set "@#{var}", original.send(var)
      end
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
        (wearable ? 'yes' : 'no'), (wearable ? slot.to_s : 'N/A'),
      ]
    end
  end
end
