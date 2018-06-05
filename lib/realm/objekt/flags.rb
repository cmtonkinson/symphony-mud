module Realm
  class Objekt
    ###########################################################################
    def self.simple_flag(flag_name, default, test_true = nil, set = nil, test_false = nil, clear = nil)
      ivar = "@#{flag_name}"

      self.class_eval do
        attr_accessor flag_name, "default_#{flag_name}"

        define_method(test_true)  { instance_variable_get ivar }        if test_true
        define_method(set)        { instance_variable_set ivar, true }  if set
        define_method(test_false) { !instance_variable_get ivar }       if test_false
        define_method(clear)      { instance_variable_set ivar, false } if clear

        after_initialize -> {
          instance_variable_set "@default_#{flag_name}", default
          instance_variable_set ivar, default
        }

        on_hashify -> {
          hash = Hash.new
          hash[flag_name] = instance_variable_get ivar
          { simple_flags: hash }
        }

        on_dehashify -> hash {
          # Adding new flags to existing Objekts should be backwards compatible
          # with the on-disk format.
          return unless hash.key? :simple_flags
          hash = hash[:simple_flags]

          instance_variable_set ivar, hash[flag_name]
        }

        on_deep_copy -> parent {
          instance_variable_set(ivar, parent.instance_variable_get(ivar))
        }

      end
    end
  end
end
