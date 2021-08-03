module Realm
  class Reward < Objekt
    attr_accessor :zone
    attr_accessor :silver, :gold, :exp
    attr_accessor :item_ids

    # TODO: modifiers, effects, npcs (enemies or pets), jobs, scenes, etc.

    def initialize(id = nil, props = {})
      @zone     = nil
      @silver   = 0
      @gold     = 0
      @exp      = 0
      @item_ids = [] # rids or UUIDs
      super
    end

    after_initialize lambda {
    }

    on_hashify lambda {
      hash = {
        zone: zone.id,
        silver: silver,
        gold: gold,
        exp: exp,
        item_ids: item_ids.join(',')
      }
      { reward: hash }
    }

    on_dehashify lambda { |hash|
      hash = hash[:reward]

      @zone = $world.find hash[:zone]
      @silver = hash[:silver]
      @gold = hash[:gold]
      @exp = hash[:exp]
      @item_ids = hash[:item_ids].split(',')
    }

    on_deep_copy lambda { |original|
      @zone = original.zone
      @silver = original.silver
      @gold = original.gold
      @exp = original.exp
      @item_ids = original.item_ids
    }

    enervate lambda {
      dispose
    }

    def to_str_array
      output = []

      output << "{y#{gold}{x gold" if gold.positive?
      output << "{W#{silver}{x silver" if silver.positive?
      output << "{Y#{exp}{x experience" if exp.positive?

      item_ids.each do |item_id|
        item = $world.search zone, Realm::Item, item_id
        next if item.nil?

        output << "{M#{item.name}{x"
      end

      output
    end
  end
end
