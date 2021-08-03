module Combat
  class Damage
    attr_reader :base_amount, :type, :elements
    attr_accessor :actual, :source, :implement
    attr_accessor :armor_adjustment, :elemental_adjustment

    # d = Combat::Damage.new(53).slash.fire(20).implement(weapon).from(attacker)
    def initialize(base_amount)
      @base_amount = base_amount
      @actual      = nil
      @source      = nil
      @implement   = nil
      @type        = nil
      @elements    = {}
    end

    Konst::DAMAGE_TYPES.each do |t|
      define_method(t) do
        @type = t
      end
    end

    Konst::ELEMENTS.each do |el|
      define_method(el) do |value|
        if value.zero?
          @elements.delete el
        else
          @elements[el] = value
        end
      end
    end
  end
end
