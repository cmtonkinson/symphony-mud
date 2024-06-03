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
    attach_function :lua_pushinteger, [:pointer, :int], :void
    attach_function :lua_setglobal, [:pointer, :string], :void
    # rubocop:enable Style/SymbolArray

    # State will be the main class for consumers to interact with
    # the Lua engine. It handles state cretion and most importantly,
    # because I'm a shit architect, it also directly handles exposing
    # the myriad Symphony functions we need available in the Lua
    # environment.
    #
    # TODO: Look for a way where we can create a new state, configure the
    # environment, globals, functions, and then serialize/clone/snapshot etc
    # so we don't have to keep reinitializing everything from scratch every
    # time a new Lua script needs to be run.
    class State
      # Expects an Object (we'll call it the "given" object) that has a
      # corresponding class defined in the Scripting::Abstraction module. An
      # abstracted object will be created (wrapping the given object). Then we
      # add a generic Lua closure that will proxy methods on the abstraction.
      #
      # The whole point of this is:
      #   A) Whitelist which methods can be invoked on the given object.
      #   B) Reduce the number of FFI functions we need to add to Lua state.
      #   C) Allow metaprogramming for the management & documentation of
      #      Symphony's Lua API.
      def initialize(context)
        Symphony.logger.debug "Initializing Lua state for #{context.class.name}"
        begin
          Scripting::Abstraction.constants.each do |const|
            if context.is_a? Scripting::Abstraction.const_get(const)::MAPPED_CLASS
              @abstraction_class = Scripting::Abstraction.const_get(const)
            end
          end

          if @abstraction_class.nil?
            err = "No abstraction class found for #{context.class.name}"
            Symphony.logger.error err
            raise StandardError, err
          end
        rescue NameError
          err = "No abstraction class found for #{context.class.name}"
          Symphony.logger.error err
          raise StandardError, err
        end

        @abstraction = @abstraction_class.new context
        @state = Lua.luaL_newstate
        # TODO: Do we actually want/need All The Libs?
        Lua.luaL_openlibs(@state)
      end

      # This is a mess. Each method in the abstration class gets mapped to
      # its own Lua closure. The function has to iterate over the param
      # definitions and check the Lua stack for the correct type. If the
      # type is incorrect, we have error handling to do.
      def setup
        @abstraction_class::METHODS.each do |method|
          puts "registering method: #{method[:name]}"
          lua_method = FFI::Function.new(:int, [:pointer]) do |lua_state|
            Symphony.logger.debug "executing #{@abstraction_class}::#{method[:name]}(#{method[:params]})"
            args = []
            method[:params].each_with_index do |param, index|
              case param
              when :string
                if Lua.lua_isstring(lua_state, index + 1) == 1
                  str = Lua.luaL_tolstring lua_state, index + 1
                  args << str
                else
                  err = "Error: #{@abstraction_class}::#{method[:name]} \
                    expected string for param #{index + 1}"
                  Lua.lua_pushstring lua_state, err
                  Symphony.logger.warn err
                  return 1
                end
              # TODO: Add more types as needed
              else
                err = "Error: #{@abstraction_class}::#{method[:name]} \
                  unknown param type for param #{index + 1}"
                Lua.lua_pushstring lua_state, err
                Symphony.logger.warn err
                return 1
              end
            end

            result = @abstraction.send method[:name], *args

            case method[:return_type]
            when :void
              return 0
            when :int
              Symphony.logger.debug "returning integer: #{result}"
              Lua.lua_pushinteger lua_state, result
            when :string
              Lua.lua_pushstring lua_state, result
            else
              err = "Error: #{@abstraction_class}::#{method[:name]} \
                unknown return type"
              Lua.lua_pushstring lua_state, err
              Symphony.logger.warn err
              return 1
            end
          end

          Lua.lua_pushcclosure @state, lua_method
          Lua.lua_setglobal @state, method[:name]
        end
      end

      def load(script)
        Lua.luaL_loadstring(@state, script)
      end

      def call
        if Lua.lua_pcallk(@state, 0, 0, 0, nil, nil) != 0
          err = "Lua error: #{Lua.luaL_tolstring @state, -1}"
          Symphony.logger.warn err
        end
      end

      def close
        Lua.lua_close(@state)
      end
    end
  end
end
