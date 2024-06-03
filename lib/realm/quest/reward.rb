module Realm
  class Reward < Objekt
    attr_accessor :silver, :gold, :exp, :item_ids

    # TODO: modifiers, effects, npcs (enemies or pets), jobs, scenes, etc.

    def initialize(id = nil, props = {})
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
        silver:,
        gold:,
        exp:,
        item_ids: item_ids.join(',')
      }
      { reward: hash }
    }

    on_dehashify lambda { |hash|
      hash = hash[:reward]

      @silver = hash[:silver]
      @gold = hash[:gold]
      @exp = hash[:exp]
      @item_ids = hash[:item_ids].split(',')
    }

    on_deep_copy lambda { |original|
      @silver = original.silver
      @gold = original.gold
      @exp = original.exp
      @item_ids = original.item_ids
    }

    enervate lambda {
      dispose
    }

    def empty?
      silver.zero? && gold.zero? && exp.zero? && item_ids.empty?
    end

    def ==(other)
      silver == other.silver &&
        gold == other.gold &&
        exp == other.exp &&
        item_ids == other.item_ids
    end

    def +(other)
      res = Reward.new

      # Straight up addition for any numeric reward value.
      res.silver = silver + other.silver
      res.gold = gold + other.gold
      res.exp = exp + other.exp

      # cancat the item_ids array
      res.item_ids = item_ids + other.item_ids

      res
    end

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
