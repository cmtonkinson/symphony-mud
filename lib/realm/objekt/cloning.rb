module Realm
  class Objekt
    attr_reader :cloned_from

    ###########################################################################
    after_initialize -> {
      # When #clone is called on an Objekt, a new Objekt is returned with the
      # exact properties of the original, except two: the new Objekt has its
      # own id, and the cloned_from property will be set to the id of the
      # original.
      @cloned_from = nil
    }

    ###########################################################################
    def clone
      # Generate a new subclass-type Objekt, setting the cloned_from id.
      dolly = self.class.new
      # Deep copy Objekct properties.
      dolly.instance_variable_set :@cloned_from, id
      dolly.name = name.dup
      dolly.rid = rid.dup
      # Deep copy subclass properties.
      dolly.run_hook :on_deep_copy, self
      # Return the clone.
      dolly
    end
  end
end
