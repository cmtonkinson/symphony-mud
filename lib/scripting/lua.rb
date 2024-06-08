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
    attach_function :lua_isstring, %i[pointer int], :int
    attach_function :lua_pcallk, %i[pointer int int int pointer pointer], :int
    attach_function :lua_pushcclosure, %i[pointer pointer int], :void
    attach_function :lua_pushstring, %i[pointer string], :void
    attach_function :lua_pushinteger, %i[pointer int], :void
    attach_function :lua_setglobal, %i[pointer string], :void

    class State
      def initialize
        @state = Lua.luaL_newstate
        Lua.luaL_openlibs(@state) # TODO: Do we actually want/need All The Libs?
      end

      def script(text)
        if Lua.luaL_loadstring(@state, text).nonzero?
          err_msg = "Lua error: #{Lua.luaL_tolstring @state, -1, nil}"
          Symphony.logger.warn err_msg
          raise StandardError, err_msg
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
            err = "Lua error: #{name} unknown return type"
            Lua.lua_pushstring lua_state, err
            Symphony.logger.warn err
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
          err = "Lua error: expected string for param #{index}"
          Lua.lua_pushstring lua_state, err
          Symphony.logger.warn err
          nil
        end
      end

      def get_int_param(lua_state, index)
        if Lua.lua_isinteger(lua_state, index) == 1
          Lua.lua_tointeger lua_state, index
        else
          err = "Lua error: expected integer for param #{index}"
          Lua.lua_pushstring lua_state, err
          Symphony.logger.warn err
          nil
        end
      end

      def param_error(lua_state, index)
        err = "Lua FFI error: unknown param type for param #{index}"
        Lua.lua_pushstring lua_state, err
        Symphony.logger.warn err
        nil
      end

      def execute
        if Lua.lua_pcallk(@state, 0, 0, 0, nil, nil).nonzero?
          err_msg = "Lua error: #{Lua.luaL_tolstring @state, -1, nil}"
          Symphony.logger.warn err_msg
          raise StandardError, err_msg
        end
      end

      def close
        Lua.lua_close(@state)
      end
    end
  end
end
