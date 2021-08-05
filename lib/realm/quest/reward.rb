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

    def empty?
      silver.zero? && gold.zero? && exp.zero? && item_ids.empty?
    end

    def ==(other)
      zone == other.zone &&
        silver == other.silver &&
        gold == other.gold &&
        exp == other.exp &&
        item_ids == other.item_ids
    end

    def +(other)
      res = Reward.new

      # Use the Zone from the left-hand side.
      res.zone = zone

      # Straight up addition for any numeric reward value.
      res.silver = silver + other.silver
      res.gold = gold + other.gold
      res.exp = exp + other.exp

      # For Items, we start with the left-hand-side array. Because we used the
      # self.zone, however, we need to be careful with the IDs in other.item_ids
      # because they can contain either RIDs or UUIDs.
      #
      # If both Zones are the same, there's no problem, because the RIDs are
      # all relative to the same Zone.
      #
      # But if the Zones are not the same, then any RIDs in other.item_ids need
      # to be transformed to UUIDs since at lookup time they will be considered
      # relative to self.zone and so must use an absolute identifier.
      res.item_ids = item_ids
      if zone == other.zone
        res.item_ids += other.item_ids
      else
        other.item_ids.each do |item_id|
          res.item_ids += if item_id.is_a? Numeric
                            $world.convert_rid_to_uuid(item_id)
                          else
                            item_id
                          end
        end
      end

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
