module Scripting
  # This is the consumer-facing class for the scripting module.
  class Script
    def initialize
      @state         = Lua::State.new
      @text          = nil
      @context       = {}
      @return_values = {}
    end

    # Set the script content to be run. Successive calls will overwrite the
    # previous script.
    def script(text)
      @text = text
      @state.script text
    end

    # Creates a set of functions my combining the given name with each of the
    # mapped methods in the Abstraction correspdonding to the given target.
    #
    # Example: set_context('hero', being) will create methods in Lua like:
    #   hero_level() # int
    #   hero_name() # string
    #   hero_puts('some text') # nil
    #   etc
    def set_context(name, target)
      abstraction = Abstraction.create_abstraction target
      @context[name] = abstraction
    end

    def clear_context(name)
      @context.delete name
    end

    # Creates all the individual methods for each context key/value pair.
    def configure_environment
      Symphony.logger.debug 'Configuring Lua environment'
      @context.each do |name, abstraction|
        abstraction.class::METHODS.each do |method_cfg|
          method_name = "#{name}_#{method_cfg[:name]}"
          Symphony.logger.trace "  Creating method: #{method_name}"
          @state.setup_method method_name, method_cfg, abstraction
        end
      end
    end

    # Execute the script with the configured context.
    def run
      configure_environment

      Symphony.logger.debug 'Running script'
      # Log each line of the scrip text
      @text.each_line do |line|
        Symphony.logger.trace "  #{line}"
      end

      @state.execute
      @state.close

      @return_values = @state.return_table
    end

    # Returns any values that were returned by the script.
    def result
      @return_values
    end
  end
end
