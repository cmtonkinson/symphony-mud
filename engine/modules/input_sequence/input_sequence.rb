module InputSequence

  class InputSequence
    include AASM

    attr_reader :name

    ###########################################################################
    def initialize
      puts "InputSequence::InputSequence#initialize name='#{@name}'"
    end
  end

end
