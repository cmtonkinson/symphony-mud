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
    attach_function :luaL_tolstring, %i[pointer int pointer], :string
    attach_function :lua_close, [:pointer], :void
    attach_function :lua_getglobal, %i[pointer string], :void
    attach_function :lua_isstring, %i[pointer int], :int
    attach_function :lua_pcallk, %i[pointer int int int pointer pointer], :int
    attach_function :lua_pushcclosure, %i[pointer pointer int], :void
    attach_function :lua_pushinteger, %i[pointer int], :void
    attach_function :lua_pushnil, %i[pointer], :void
    attach_function :lua_pushstring, %i[pointer string], :void
    attach_function :lua_setfield, %i[pointer int string], :void
    attach_function :lua_setglobal, %i[pointer string], :void
    attach_function :lua_sethook, %i[pointer pointer int int], :int
    attach_function :lua_settop, %i[pointer int], :void
    attach_function :lua_error, %i[pointer], :int

    attach_function :luaopen_base, %i[pointer], :int
    attach_function :luaopen_math, %i[pointer], :int
    attach_function :luaopen_string, %i[pointer], :int
    attach_function :luaopen_table, %i[pointer], :int

    # This controls the number of instructions run between time check
    # callbascks to guard against infinite loops (or just really long
    # scripts).
    INSTRUCTION_COUNT = 1000
    HOOK_MASK         = 0x04
    SCRIPT_TIME_LIMIT = 0.50 # 50 milliseconds

    DISALLOWED_FUNCTIONS = %w[
      dofile
      loadfile
      print
    ].freeze

    class State
      def initialize
        # Allocate a new Lua context.
        @state = Lua.luaL_newstate

        # Load default libraries.
        Lua.luaopen_base @state
        Lua.luaopen_table @state
        Lua.luaopen_math @state
        Lua.luaopen_string @state

        # Disable OS and I/O functions.
        Lua.lua_getglobal @state, '_G'
        DISALLOWED_FUNCTIONS.each do |fn|
          Lua.lua_pushnil @state
          Lua.lua_setfield @state, -2, fn
        end
        Lua.lua_settop @state, -1

        # Create a timeout hook.
        # @end_time needs to be set just before pcallk is invoked.
        @end_time = 0
        check_time = FFI::Function.new(:void, %i[pointer]) do |lua_state|
          if Time.now >= @end_time
            err = 'Lua TimeoutError: script execution took too long'
            Lua.lua_pushstring lua_state, err
            Lua.lua_error lua_state
            Symphony.logger.warn err
            raise Scripting::TimeoutError, err
          end
        end
        Lua.lua_sethook @state, check_time, HOOK_MASK, INSTRUCTION_COUNT
      end

      def script(text)
        wrapped = <<~LUA
          function wrapper()
            #{text}
          end
          wrapper()
        LUA
        if Lua.luaL_loadstring(@state, wrapped).nonzero?
          err_msg = "Lua ParsingError: #{Lua.luaL_tolstring @state, -1, nil}"
          Symphony.logger.warn err_msg
          raise Scripting::ParsingError, err_msg
        end
      end

      def setup_method(name, config, abstraction)
        # This is the closure that will be called by Lua when the method is
        # invoked.
        lua_closure = FFI::Function.new(:int, [:pointer]) do |lua_state|
          Symphony.logger.trace "Executing #{name}(#{config[:params]})"
          args = []

          # Because the C API is typed (duh) we need to check each parameter
          # type manually and grab it with a special function.
          config[:params].each_with_index do |param, index|
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

          # Like parameters, we have to handle each return type manually
          # because the C API is typed (duh (again)).
          case config[:return_type]
          when :void
            # NOOP
          when :int
            Lua.lua_pushinteger lua_state, result
          when :string
            Lua.lua_pushstring lua_state, result
          else
            # Oh noes! Unknown return type!
            err = "Lua ReturnTypeError: #{name} unknown return type"
            Lua.lua_pushstring lua_state, err
            Symphony.logger.warn err
            raise Scripting::ReturnTypeError, err
          end
        end

        # With the closure defined, we can push it to the Lua stack and set it.
        Lua.lua_pushcclosure @state, lua_closure, 0
        Lua.lua_setglobal @state, name
      end

      def get_string_param(lua_state, index)
        if Lua.lua_isstring(lua_state, index) == 1
          Lua.luaL_tolstring lua_state, index, nil
        else
          err = "Lua ArgumentError: expected string for param #{index}"
          Lua.lua_pushstring lua_state, err
          Symphony.logger.warn err
          raise Scripting::ArgumentError, err
        end
      end

      def get_int_param(lua_state, index)
        if Lua.lua_isinteger(lua_state, index) == 1
          Lua.lua_tointeger lua_state, index
        else
          err = "Lua ArgumentError: expected integer for param #{index}"
          Lua.lua_pushstring lua_state, err
          Symphony.logger.warn err
          raise Scripting::ArgumentError, err
        end
      end

      def param_error(lua_state, index)
        err = "Lua ArgumentError: unknown param type for param #{index}"
        Lua.lua_pushstring lua_state, err
        Symphony.logger.warn err
        raise Scripting::ArgumentError, err
      end

      def execute
        @end_time = Time.now + SCRIPT_TIME_LIMIT
        if Lua.lua_pcallk(@state, 0, 0, 0, nil, nil).nonzero?
          err_msg = "Lua RuntimeError: #{Lua.luaL_tolstring @state, -1, nil}"
          Symphony.logger.warn err_msg
          raise Scripting::RuntimeError, err_msg
        end
      end

      def close
        Lua.lua_close(@state)
      end
    end
  end
end
