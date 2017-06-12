module Realm
  class Objekt

    define_hooks :before_remove, :after_remove
    define_hooks :before_add, :after_add

    ###########################################################################
    def add
      Logistics::Adder.new self
    end

    ###########################################################################
    def remove
      Logistics::Remover.new self
    end

    ###########################################################################
    def move
      Logistics::Mover.new self
    end

  end
end

