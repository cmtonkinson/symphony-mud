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

    # rubocop:disable Style/SymbolArray
    attach_function :luaL_loadstring, [:pointer, :string], :int
    attach_function :luaL_newstate, [], :pointer
    attach_function :luaL_openlibs, [:pointer], :void
    attach_function :luaL_tolstring, [:pointer, :int], :string
    attach_function :lua_close, [:pointer], :void
    attach_function :lua_isstring, [:pointer, :int], :int
    attach_function :lua_pcallk, [:pointer, :int, :int, :int, :pointer, :pointer], :int
    attach_function :lua_pushcclosure, [:pointer, :pointer], :void
    attach_function :lua_pushstring, [:pointer, :string], :void
    attach_function :lua_setglobal, [:pointer, :string], :void
    # rubocop:enable Style/SymbolArray
  end
end

module Scripting
  module Lua
    # State will be the main class for consumers to interact with
    # the Lua engine. It handles state cretion and most importantly,
    # because I'm a shit architect, it also directly handles exposing
    # the myriad Symphony functions we need available in the Lua
    # environment.
    #
    # TODO: Consider another abstraction to manage mapping in all the methods.
    # TODO: Look for a way where we can create a new state, configure the
    # environment, globals, functions, and then serialize/clone/snapshot etc
    # so we don't have to keep reinitializing everything from scratch every
    # time a new Lua script needs to be run.
    class State
      def initialize
        @state = Lua.luaL_newstate
        Lua.luaL_openlibs(@state)

        # rubocop:disable Lint/NestedMethodDefinition
        def ruby_do_stuff(str)
          puts "Hello from Ruby! Your argument was: >>>#{str}<<<"
        end
        # rubocop:enable Lint/NestedMethodDefinition

        lua_do_stuff = FFI::Function.new(:int, [:pointer]) do |lua_state|
          if Lua.lua_isstring(lua_state, 1) == 1
            str = Lua.luaL_tolstring lua_state, 1
            result = ruby_do_stuff str
            Lua.lua_pushstring lua_state, result
          else
            Lua.lua_pushstring lua_state, 'Error: expected string'
          end
          1
        end

        Lua.lua_pushcclosure(@state, lua_do_stuff)
        Lua.lua_setglobal(@state, 'lua_do_stuff')
      end

      def load(script)
        Lua.luaL_loadstring(@state, script)
      end

      def call
        Lua.lua_pcallk(@state, 0, 0, 0, nil, nil)
      end

      def close
        Lua.lua_close(@state)
      end
    end
  end
end
