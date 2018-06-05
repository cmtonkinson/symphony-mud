require 'ostruct'

module InputSequence
  class InputSequence
    include AASM

    attr_reader   :name, :context
    attr_accessor :user

    ###########################################################################
    def initialize
      @name    = nil
      @context = OpenStruct.new
    end

    ###########################################################################
    def handle(input)
      Symphony.logger.trace "#{self.class.name} in state '#{aasm.current_state}' handling '#{input}'"
      send "#{aasm.current_state}_handler", input
    end

    #########################################################################
    aasm do
      after_all_transitions :log_status_change
      state :_pending, initial: true
      state :_finished

      event :reset do
        transitions to: :_pending
        after do
          _start
        end
      end

      event :finish do
        transitions to: :_finished
        after do
          user.operators.pop
        end
      end
    end

    #########################################################################
    def log_status_change
      str = self.class.name
      str << ' transitioning '
      str << "from '#{aasm.from_state}' " if aasm.from_state != nil
      str << "to '#{aasm.to_state}' "
      str << "(on '#{aasm.current_event}')"
      Symphony.logger.trace str
    end
  end
end
