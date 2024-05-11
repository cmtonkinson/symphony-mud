# This class contains the configuration & code for implementing telnet
# options negotiation, and stores connection-specific data relating to
# a specific network Client instance.
module Network
  class Telnet
    IAC            = 255 # command marker (Interpret As Command)

    CMD_SUB_END    = 240 # Subnegotiation end
    CMD_NOOP       = 241 # No operation
    CMD_AYT        = 246 # Are You There?
    CMD_SUB_START  = 250 # Subnegotiation begin
    CMD_WILL       = 251 # Sender would like to use an option
    CMD_WONT       = 252 # Sender will not use an option
    CMD_DO         = 253 # Tell sender to use an option
    CMD_DONT       = 254 # Tell sender not to use option

    OPT_ECHO       = 1 # Echo
    OPT_SGA        = 3 # Suppress Go Ahead
    OPT_STATUS     = 5 # Status
    OPT_TIME       = 6 # Timing Mark
    OPT_LINE_WIDTH = 8 # Output Line Width
    OPT_PAGE_SIZE  = 9 # Output Page Size
    OPT_TTYPE      = 24 # Terminal Type
    OPT_EOR        = 25 # End of Record
    OPT_WIN_SIZE   = 31 # Window Size
    OPT_TSPEED     = 32 # Terminal Speed
    OPT_LINEMODE   = 34 # Linemode

    attr_accessor :client, :values

    # Saves a reference to the Network::Client and then initializes a blank
    # map of values for each option.
    def initialize(client)
      @client = client
      @values = {}
    end

    # Validate that the client respects the commands we care about
    # and attempt to set defaults.
    def negotiate
      send_command CMD_DO, OPT_WIN_SIZE
    end

    # Disable local echo (mainly for password entry)
    # NOTE: strange thing about ECHO, telling the client the server WILL ECHO
    # seems be work more reliably than telling the client DONT ECHO.
    def echo_off
      send_command CMD_WILL, OPT_ECHO
    end

    # Enable local echo
    def echo_on
      send_command CMD_WONT, OPT_ECHO
    end

    # Enable interactive mode
    def interactive_on
      send_command CMD_WILL, OPT_SGA
    end

    # Disable interactive mode
    def interactive_off
      send_command CMD_WONT, OPT_SGA
    end

    # Send a command or response to the client.
    def send_command(cmd, opt, *data)
      dgram = [IAC, cmd, opt, *data]
      Symphony.logger.debug "Sending TELNET command #{dgram.join(',')} to socket #{@client.fd}"
      @client.socket.print dgram.pack('C*')
      @client.socket.flush
    end

    # Parse a command or response from the client.
    def recv_command(input)
      bytes = input.unpack('C*')
      Symphony.logger.trace "Recieved #{bytes.length}-byte TELNET command #{bytes.join(',')} on socket #{@client.fd}"

      # At this point, the first byte should be IAC.
      # Treating this as an invariant for all recieved Telnet commands.
      iac = bytes[0]
      unless iac == IAC
        Symphony.logger.debug "Telnet - first byte != IAC (expected #{IAC} got #{iac}). Rest was #{bytes.join(',')}"
        return
      end

      # Parse remaining input.
      interpret_command input, bytes
    end

    # Where we actually do something with received commands.
    # rubocop:disable Metrics/MethodLength
    def interpret_command(input, bytes)
      # We will use the bytes_consumed variable after the nested case to eat
      # the appropriate maount of data out of the input. This is because telnet
      # can actually send multiple commands in a single datagram. But since we
      # don't know how to interpret the data before we start parsing it, once
      # we're done we have to convert the whole thing to a byte array, shift
      # off the ones we used, and then repack it and recurse.
      bytes_consumed = 0
      cmd = bytes[1]

      case cmd
      when CMD_WILL, CMD_WONT
        opt = bytes[2]
        case opt
        when OPT_WIN_SIZE
          bytes_consumed = 3
        else
          Symphony.logger.warn "Telnet recieved unsupported option #{opt} (#{bytes.join(',')})."
        end
      when CMD_DO, CMD_DONT
        opt = bytes[2]
        case opt
        when OPT_ECHO
          bytes_consumed = 3
        else
          Symphony.logger.warn "Telnet received unsupported option #{opt} (#{bytes.join(',')})."
        end
      when CMD_SUB_START
        opt = bytes[2]
        case opt
        when OPT_WIN_SIZE
          data = input.unpack('CCCnnCC')
          if data[5] != IAC || data[6] != CMD_SUB_END
            Symphony.logger.warn "Telnet subnegotiation for NAWS did not end with IAC/SE (#{bytes.join(',')})."
            return
          end
          @values[:cols] = data[3]
          @values[:rows] = data[4]
          bytes_consumed = 9
          Symphony.logger.trace "Client output view changed to #{@values[:cols]}x#{@values[:rows]}."
        else
          Symphony.logger.warn "Telnet recieved unsupported subnegotiation #{opt} (#{bytes.join(',')})"
        end
      else
        Symphony.logger.warn "Telnet recieved unsupported command #{cmd} (#{bytes.join(',')})"
      end

      # Sometimes the client will stack multiple commands in the same input so
      # we have to eat whatever bytes we used and recurse into the parser again
      # when that happens.
      return unless bytes_consumed.positive? && bytes.length > bytes_consumed

      Symphony.logger.trace "Consumed #{bytes_consumed} bytes of #{bytes.length}. Recursing."
      bytes.shift(bytes_consumed)
      recv_command bytes.pack('C*')
    end
  end
  # rubocop:enable Metrics/MethodLength
end
