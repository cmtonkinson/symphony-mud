module Scripting
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
    local _symphony_time_limit = 0
    local function _symphony_check_time()
    if os.clock() >= _symphony_time_limit then
        error('#{SCRIPT_TIME_ERROR}')
    end
    end

    -- Sandbox the user script.
    do
    local _ENV = {
        -- Expose the basic Lua functions we want to allow.
        assert   = assert,
        error    = error,
        ipairs   = ipairs,
        next     = next,
        pairs    = pairs,
        select   = select,
        tonumber = tonumber,
        tostring = tostring,
        type     = type,
        -- Expose the modules we want to allow full access to.
        math      = math,
        string    = string,
        table     = table,
        -- Expose just the date/time functions from the os module.
        os = {
        clock    = os.clock,
        date     = os.date,
        difftime = os.difftime,
        time     = os.time,
        },
        -- I'd prefer not to expose coroutine and debug to the client
        -- script, but I don't see a way around it right now.
        coroutine = {
        wrap   = coroutine.wrap,
        resume = coroutine.resume,
        yield  = coroutine.yield,
        close  = coroutine.close,
        },
        debug = {
        sethook = debug.sethook
        },
        -- Need to expose the time limit variable for proper loop control.
        -- This is because every time a coroutine.yield() returns, we need
        -- to reset the time limit.
        _symphony_time_limit = _symphony_time_limit,
        -- The script return table allows the script to share primitive
        -- variables back to Ruby. And the internal functions need to be
        -- in scope for the script to call them.
        _symphony_return = {},
        _symphony_return_boolean = _symphony_return_boolean,
        _symphony_return_number  = _symphony_return_number,
        _symphony_return_string  = _symphony_return_string,
        -- This is where we inject any user-defined variables.
        %s
        -- This is where we inject the user-defined functions.
        %s
    }

    -- Limit execution time.
    _symphony_time_limit = os.clock() + #{SCRIPT_TIME_LIMIT}
    debug.sethook(_symphony_check_time, 'l', #{SCRIPT_TIME_CHECK_INTERVAL})

    -- Run the actual user script.
    %s

    -- Loop through the return table and send each value back to Ruby.
    for k, v in pairs(_symphony_return) do
        if type(v) == 'boolean' then
        _symphony_return_boolean(k, v and 1 or 0)
        elseif type(v) == 'number' then
        _symphony_return_number(k, v)
        elseif type(v) == 'string' then
        _symphony_return_string(k, v)
        else
        error('Unknown return type')
        end
    end
    end
  LUA
end
