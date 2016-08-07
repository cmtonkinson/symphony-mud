module Realm
  class Objekt

    attr_reader :cloned_from

    ###########################################################################
    def clone
      raise NotImplementedError.new "#{self.class.name}#clone called but #_deep_copy is not implemented" unless respond_to? :_deep_copy
      # Generate a new subclass-type Objekt, setting the cloned_from id.
      dolly = self.class.new
      dolly.instance_variable_set :@cloned_from, id
      # Deep copy Objekct properties.
      deep_copy dolly
      # Deep copy subclass properties.
      _deep_copy dolly
      # Return the clone.
      dolly
    end

    ###########################################################################
    def deep_copy(other)
      other.name = name.dup
    end

  end
end
