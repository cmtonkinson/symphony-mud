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

    attach_function :luaL_loadstring , %i[pointer string], :int
    attach_function :luaL_newstate   , [], :pointer
    attach_function :luaL_openlibs   , [:pointer], :void
    attach_function :luaL_tolstring  , %i[pointer int pointer], :string
    attach_function :lua_close       , [:pointer], :void
    attach_function :lua_getglobal   , %i[pointer string], :void
    attach_function :lua_isstring    , %i[pointer int], :int
    attach_function :lua_pcallk      , %i[pointer int int int pointer pointer], :int
    attach_function :lua_pushcclosure, %i[pointer pointer int], :void
    attach_function :lua_pushinteger , %i[pointer int], :void
    attach_function :lua_pushnil     , %i[pointer], :void
    attach_function :lua_pushstring  , %i[pointer string], :void
    attach_function :lua_setfield    , %i[pointer int string], :void
    attach_function :lua_setglobal   , %i[pointer string], :void
    attach_function :lua_settop      , %i[pointer int], :void

    # This is the maximum time a script is allowed to run (in seconds).
    SCRIPT_TIME_LIMIT = 0.05

    # This is the number of lines of code after which the time limit check
    # will be performed.
    SCRIPT_TIME_CHECK_INTERVAL = 1000

    # Error messages for a script that exceeds the time limit.
    SCRIPT_TIME_ERROR = 'Script execution time exceeded'

    # This is the Lua code that will be injected into the user script to
    # enforce the time limit.
    SCRIPT_WRAPPER = <<-LUA
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
          math   = math,
          string = string,
          table  = table,
          os     = {
            clock    = os.clock,      
            date     = os.date,       
            difftime = os.difftime,
            time     = os.time,       
          },
        }
        %s
      end
    LUA

    class State
      def initialize
        # Allocate a new Lua context with libraries open.
        @state = Lua.luaL_newstate
        Lua.luaL_openlibs @state
      end

      def script(user_script)
        wrapped = SCRIPT_WRAPPER % [user_script]

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

      def close
        Lua.lua_close(@state)
      end
    end
  end
end
