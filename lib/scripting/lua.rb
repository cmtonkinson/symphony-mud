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

    attach_function :luaL_loadstring, [:pointer, :string], :int
    attach_function :luaL_newstate, [], :pointer
    attach_function :luaL_openlibs, [:pointer], :void
    attach_function :luaL_tolstring, [:pointer, :int, :pointer], :string
    attach_function :lua_close, [:pointer], :void
    attach_function :lua_isstring, [:pointer, :int], :int
    attach_function :lua_pcallk, [:pointer, :int, :int, :int, :pointer, :pointer], :int
    attach_function :lua_pushcclosure, [:pointer, :pointer, :int], :void
    attach_function :lua_pushstring, [:pointer, :string], :void
    attach_function :lua_pushinteger, [:pointer, :int], :void
    attach_function :lua_setglobal, [:pointer, :string], :void

    class State
      def initialize
        @state = Lua.luaL_newstate
        Lua.luaL_openlibs(@state) # TODO: Do we actually want/need All The Libs?
      end

      def set_script(text)
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
          Symphony.logger.trace "executing #{name}(#{config[:params]})"
          args = []

          # Because the C API is typed (duh) we need to check each parameter
          # type manually.
          config[:params].each_with_index do |param, index|
            case param
            when :string
              if Lua.lua_isstring(lua_state, index + 1) == 1
                str = Lua.luaL_tolstring lua_state, index + 1, nil
                args << str
              else
                err = "Error: #{name} expected string for param #{index + 1}"
                Lua.lua_pushstring lua_state, err
                Symphony.logger.warn err
                return 1
              end
            when :int
              if Lua.lua_isinteger(lua_state, index + 1) == 1
                int = Lua.lua_tointegerx lua_state, index + 1
                args << int
              else
                err = "Error: #{name} expected integer for param #{index + 1}"
                Lua.lua_pushstring lua_state, err
                Symphony.logger.warn err
                return 1
              end
            else
              err = "Error: #{name} unknown param type for param #{index + 1}"
              Lua.lua_pushstring lua_state, err
              Symphony.logger.warn err
              return 1
            end
          end

          # With the appropriate parameters in the right place, we can invoke
          # the method against the abstraction.
          result = abstraction.send config[:name], *args

          # Like parameters, we have to handle each return type manually.
          case config[:return_type]
          when :void
            # NOOP
          when :int
            Lua.lua_pushinteger lua_state, result
          when :string
            Lua.lua_pushstring lua_state, result
          else
            err = "Error: #{name} unknown return type"
            Lua.lua_pushstring lua_state, err
            Symphony.logger.warn err
          end
        end

        # With the closure defined, we can push it to the Lua stack and set it.
        Lua.lua_pushcclosure @state, lua_closure, 0
        Lua.lua_setglobal @state, name
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
