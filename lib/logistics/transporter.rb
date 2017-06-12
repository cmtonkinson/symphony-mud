module Logistics

  class Transporter
    attr_reader :quarry, :source, :destination, :operation

    ###########################################################################
    def initialize(objekt, operation)
      @quarry      = objekt
      @source      = nil
      @destination = nil
      @operation   = operation
    end

    ###########################################################################
    def to(target)
      @destination = target
      resolve
    end

    ###########################################################################
    def from(target)
      @source = target
      resolve
    end

    ###########################################################################
    def resolve
      return self if operation != :add && source.nil?
      return self if operation != :remove && destination.nil?

      unless operation == :add
        quarry.run_hook :before_remove
        source.remove quarry unless operation == :add
        quarry.run_hook :after_remove
      end

      unless operation == :remove
        quarry.run_hook :before_add
        destination.insert quarry unless operation == :remove
        quarry.run_hook :after_add
      end

      return nil
    end

  end

  class Adder < Transporter
    def initialize(objekt)
      super objekt, :add
    end
  end

  class Remover < Transporter
    def initialize(objekt)
      super objekt, :remove
    end
  end

  class Mover < Transporter
    def initialize(objekt)
      super objekt, :move
    end
  end

end
