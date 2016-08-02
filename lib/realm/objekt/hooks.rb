module Realm

  class Objekt

    HOOKS = %i(after_initialize)

    # Define the main hooking interface for Objekts. Using the hooking system
    # is a three step process:
    # 1. Call `hook :my_custom_hook` on your class. This will create a class
    #    method called `my_custom_hook` that you can use to add callbacks to
    #    be invoked when the hook is triggered.
    # 2. Send your class the method `my_custom_hook` and provide a callback
    #    Proc. The Proc should accept one parameter, an instance of your class.
    #    You may add as many callbacks as you like for any given hook and they
    #    will be invoked in the order you added them.
    # 3. Invoke hooks by calling `trigger(:my_custom_hook)` on an instance of
    #    your class.
    #
    # NOTE: Hooks defined on a base class (such as Objekt) will be available
    # for use on subclasses (such as Avatar). Callbacks added on a base class
    # will NOT be run when triggering the hook on an instance of the subclass.
    class << self
      # This is the class method used to declare a new hook.
      def hook(hook_name)
        #######################################################################
        # This method ensures that a class instance variable called `@hooks`
        # exists and defaults to an empty Hash.
        def hooks
          @hooks ||= Hash.new
        end

        #######################################################################
        # This method defines the hook declaration class method, and ensures
        # that an Array exists to hold the callbacks.
        self.class.send :define_method, hook_name, Proc.new { |callback|
          @hooks = hooks
          @hooks[hook_name] ||= []
          @hooks[hook_name] << callback
        }
      end
    end

    HOOKS.each { |hook_name| hook hook_name }

    ###########################################################################
    def trigger(hook_name)
      return unless self.class.hooks.key? hook_name
      self.class.hooks[hook_name].each do |hook_proc|
        hook_proc.call self
      end
    end

    ###########################################################################
    def self.simple_flag(flag_name, default, test_true = nil, set = nil, test_false = nil, clear = nil)
      self.class_eval do
        attr_accessor flag_name, "default_#{flag_name}"
        define_method(test_true) { instance_variable_get "@#{flag_name}" } if test_true
        define_method(set) { instance_variable_set "@#{flag_name}", true } if set
        define_method(test_false) { !instance_variable_get "@#{flag_name}" } if test_false
        define_method(clear) { instance_variable_set "@#{flag_name}", false } if clear
        after_initialize -> obj {
          obj.instance_variable_set "@default_#{flag_name}", default
          obj.instance_variable_set "@#{flag_name}", default
        }
      end
    end

  end

end
