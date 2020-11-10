module Combat
  class Damage
    attr_reader :base_amount, :source

    # type (bash, slash, pierce, exotic)
    # implement (e.g. the weapon)
    # alignment
    # element
    # direct? (e.g. sword slash or magic missle vs AoE or DoT)

    TYPES = %i[slash bash pierce exotic].freeze
    ELEMENTS = %i[wind water ice fire earth].freeze

    ###########################################################################
    def initialize(base_amount)
      @type = nil
      @alignment = 0.0
      @base_amount = base_amount
    end

    ###########################################################################
    def apply; end

    ###########################################################################
    TYPES.each do |t|
      define_method(t) do
        @type = t
      end
    end
  end
end
