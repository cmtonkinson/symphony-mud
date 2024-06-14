# I have literally no idea what I'm doing here.
# My grasp on FFI is tenuous at best, and I know nothing of Lua.
# I'm just following examples and hoping for the best.
# You have been warned. "It works on my machine!"
#
# TODO: No effs are given herein regarding security implications.
# TODO: Ditto for performance, error handling, and memory management.
#
# Seriously, for all you know I'm a dog on a keyboard.

module Scripting
  # This is the main FFI hook for the Lua module. It's honestly
  # kind of annoying to have to enumerate every function like this
  # but I'm not sure there's a better option. For now I'm just
  # keeping a straight list in alphabetical order.
  module Lua
    extend FFI::Library
    ffi_lib 'lua5.4'

    attach_function :luaL_loadstring, %i[pointer string], :int
    attach_function :luaL_newstate, [], :pointer
    attach_function :luaL_openlibs, [:pointer], :void
    attach_function :luaL_tolstring, %i[pointer int pointer], :string
    attach_function :lua_close, [:pointer], :void
    attach_function :lua_isinteger, %i[pointer int], :int
    attach_function :lua_isstring, %i[pointer int], :int
    attach_function :lua_pcallk, %i[pointer int int int pointer pointer], :int
    attach_function :lua_pushcclosure, %i[pointer pointer int], :void
    attach_function :lua_pushinteger, %i[pointer int], :void
    attach_function :lua_pushstring, %i[pointer string], :void
    attach_function :lua_setglobal, %i[pointer string], :void

    # This is the maximum time a script is allowed to run (in seconds).
    SCRIPT_TIME_LIMIT = 0.05

    # This is the number of lines of code after which the time limit check
    # will be performed.
    SCRIPT_TIME_CHECK_INTERVAL = 1000

    # Error messages for a script that exceeds the time limit.
    SCRIPT_TIME_ERROR = 'Script execution time exceeded'.freeze

    # This is the Lua code that will be injected into the user script to
    # enforce the time limit.
    SCRIPT_WRAPPER = <<-LUA.freeze
      -- Limit total script execution time.
      local _symphony_time_limit = os.clock() + #{SCRIPT_TIME_LIMIT}
      local function _symphony_check_time()
        if os.clock() >= _symphony_time_limit then
          error('#{SCRIPT_TIME_ERROR}')
        end
      end
      debug.sethook(_symphony_check_time, 'l', #{SCRIPT_TIME_CHECK_INTERVAL})

      -- Sandbox the user script.
      do
        local _ENV = {
          -- Expose the libraries we want to allow.
          math   = math,
          string = string,
          table  = table,
          -- We don't want to expose all of the os library, so we manually
          -- allow specific functions.
          os     = {
            clock    = os.clock,
            date     = os.date,
            difftime = os.difftime,
            time     = os.time,
          },
          -- This is where we inject the user-defined methods.
          %s
        }
        -- This is where the user script is injected.
        %s
      end
    LUA

    class State
      def initialize
        # The user provided script.
        @raw_script = ''

        # Because of how we restrict the Lua _ENV, we need to keep track of
        # the method names that are defined so we can allow them to be called.
        @method_names = Set.new

        # Allocate a new Lua context with libraries open.
        @state = Lua.luaL_newstate
        Lua.luaL_openlibs @state
      end

      # Sets the user script to be executed.
      def script(user_script)
        @raw_script = user_script
      end

      # This method prefixes the user script with the necessary code to
      # enforce the time limit and sandboxing. Then it loads the script into
      # the Lua environment.
      def wrap_script
        name_mapping = @method_names.map { |name| "#{name} = #{name}," }.join("\n")
        wrapped      = SCRIPT_WRAPPER % [name_mapping, @raw_script]

        if Lua.luaL_loadstring(@state, wrapped).nonzero?
          err_msg = "Lua ParsingError: #{Lua.luaL_tolstring @state, -1, nil}"
          Symphony.logger.warn err_msg
          raise Scripting::ParsingError, err_msg
        end
      end

      # This method creates the actual underlying Lua method corresponding to
      # the method defined in the abstraction.
      def setup_method(name, config, abstraction)
        @method_names.add name

        # This is the closure that will be called by Lua when the method is
        # invoked.
        lua_closure = FFI::Function.new(:int, [:pointer]) do |lua_state|
          # Reset the trace variables.
          @trace_object      = nil
          @trace_method      = nil
          @trace_args        = nil
          @trace_return_type = nil
          @trace_return      = nil

          Symphony.logger.trace "Executing #{name}(#{config[:params]})"
          @trace_object      = abstraction.underlying.class.name
          @trace_method      = config[:name]
          @trace_args        = config[:params].map { |p| ":#{p}" }.join(',')
          @trace_return_type = config[:return_type]
          args = []

          # Because the C API is typed (duh) we need to check each parameter
          # type manually and grab it with a special function.
          config[:params].each_with_index do |param, index|
            puts index
            puts param.inspect
            type = Lua.lua_type lua_state, index + 1
            typename = Lua.lua_typename lua_state, type
            puts type
            puts typename

            case param
            when :string
              args << get_string_param(lua_state, index + 1)
            when :int
              args << get_int_param(lua_state, index + 1)
            else
              # Oh noes! Unknown param type!
              param_error(lua_state, index + 1)
            end
          end

          # With the appropriate parameters in the right order, we can invoke
          # the designated method against the abstraction.
          result = abstraction.send config[:name], *args
          @trace_return = result
          Symphony.logger.trace "Result: #{result}"

          # Like parameters, we have to handle each return type manually
          # because the C API is typed (duh (again)).
          # Remember that the return value is the number of items pushed to
          # the stack.
          case config[:return_type]
          when :void
            0
          when :int
            Lua.lua_pushinteger lua_state, result
            1
          when :string
            Lua.lua_pushstring lua_state, result
            1
          else
            # Oh noes! Unknown return type!
            err = "Lua ReturnTypeError: #{name} unknown return type :: #{trace_string}"
            Lua.lua_pushstring lua_state, err
            Symphony.logger.warn err
            raise Scripting::ReturnTypeError, err
          end
        end

        # With the closure defined, we can push it to the Lua stack and set it.
        Lua.lua_pushcclosure @state, lua_closure, 0
        Lua.lua_setglobal @state, name
      end

      # This method is used to get string parameters from the Lua stack.
      def get_string_param(lua_state, index)
        if Lua.lua_isstring(lua_state, index) == 1
          Lua.luaL_tolstring lua_state, index, nil
        else
          err = "Lua ArgumentError: expected string for param #{index} :: #{trace_string}"
          Lua.lua_pushstring lua_state, err
          Symphony.logger.warn err
          raise Scripting::ArgumentError, err
        end
      end

      # This method is used to get integer parameters from the Lua stack.
      def get_int_param(lua_state, index)
        if Lua.lua_isinteger(lua_state, index) == 1
          Lua.lua_tointeger lua_state, index
        else
          err = "Lua ArgumentError: expected integer for param #{index} :: #{trace_string}"
          Lua.lua_pushstring lua_state, err
          Symphony.logger.warn err
          raise Scripting::ArgumentError, err
        end
      end

      # This is the error message for when we encounter an unknown parameter.
      def param_error(lua_state, index)
        err = "Lua ArgumentError: unknown param type for param #{index} :: #{trace_string}"
        Lua.lua_pushstring lua_state, err
        Symphony.logger.warn err
        raise Scripting::ArgumentError, err
      end

      # Finish setting up the Lua environment and execute the script.
      def execute
        wrap_script

        if Lua.lua_pcallk(@state, 0, 0, 0, nil, nil).nonzero?
          raw_err = Lua.luaL_tolstring @state, -1, nil
          if raw_err.include? SCRIPT_TIME_ERROR
            err_msg = "Lua TimeoutError: #{raw_err}"
            Symphony.logger.warn err_msg
            raise Scripting::TimeoutError, err_msg
          else
            err_msg = "Lua RuntimeError: #{raw_err}"
            Symphony.logger.warn err_msg
            raise Scripting::RuntimeError, err_msg
          end
        end
      end

      # Clean up the Lua environment.
      def close
        Lua.lua_close(@state)
      end

      # This just returns a nicely formatted string to help with script
      # debuggin.
      def trace_string
        "%s#%s(%s) -> %s:%s" % [@trace_object,
                                @trace_method,
                                @trace_args,
                                @trace_return_type,
                                @trace_return]
      end
    end
  end
end
