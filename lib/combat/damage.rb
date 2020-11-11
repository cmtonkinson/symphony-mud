module Combat
  class Damage
    attr_reader :base_amount, :source

    # type (bash, slash, pierce, exotic)
    # implement (e.g. the weapon)
    # alignment
    # element
    # direct? (e.g. sword slash or magic missle vs AoE or DoT)

    TYPES = %i[slash bash pierce exotic].freeze

    ###########################################################################
    # d = Combat::Damage.new(53).slash.fire(20).implement(weapon).from(attacker)
    def initialize(base_amount)
      @base_amount = base_amount
      @type = nil
      @elements = {}
    end

    ###########################################################################
    def apply; end

    ###########################################################################
    TYPES.each do |t|
      define_method(t) do
        @type = t
        self
      end
    end

    ###########################################################################
    # Realm::ELEMENTS.each do |el|
    #   define_method(el) do |value|
    #     @elements[el] = value
    #     self
    #   end
    # end
  end
end
