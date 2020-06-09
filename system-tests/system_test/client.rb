require_relative '../../lib/base/terminal'
require 'net-telnet'

module SystemTest
  class Client
    attr_accessor :account, :buffer

    ###########################################################################
    def initialize(port, state_path, account_name)
      @buffer = []
      @account = account_name.to_s
      @log_path = state_path.join("#{account}-log.txt").to_s

      @conn = Net::Telnet.new(
        'Hostname' => 'localhost',
        'Port' => port,
        'Prompt' => /./,
        'Waittime' => $TELNET_DELAY
      )

      # We need to eat the welcome banner or suffer a fencepost error in the
      # input/output stream processing. I could figure this out and develop
      # a more elegant solution buuuuuuuuut...
      @conn.waitfor(/You have connected to the server/)
    end

    ###########################################################################
    def close
      @conn.close
    end

    ###########################################################################
    def password
      'superSECURE!!!!passphrase123456789'
    end

    ###########################################################################
    def command(str)
      retval = nil
      IO.write @log_path, "\n$> #{str}\n", mode: 'a'
      @conn.cmd(str) do |c|
        retval = ::Base::Terminal.unescape(c)
        @buffer << retval
        IO.write @log_path, retval, mode: 'a'
      end
      retval
    end

    ###########################################################################
    # Busywait for input we may have recieved, and log it. Timeout is very low
    # because (theoretically) this will get called when there is suspicion
    # that some input either is already waiting for the client, or none will
    # come.
    def poll
      @conn.waitfor('Match' => /./, 'Timeout' => $TELNET_DELAY) do |c|
        retval = ::Base::Terminal.unescape(c)
        @buffer << retval
        IO.write @log_path, retval, mode: 'a'
      end
    # Assuming the client recieved no new data, eat the timeout exception.
    rescue Net::ReadTimeout # rubocop:disable Lint/SuppressedException
    end
  end
end
