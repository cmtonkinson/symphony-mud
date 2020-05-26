require_relative '../../lib/base/terminal'
require 'net-telnet'

module SystemTest
  class Client
    attr_accessor :account, :buffer

    ###########################################################################
    def initialize(port, state_path, account_name)
      @buffer = []
      @account = account_name
      @log_path = state_path.join("#{account}-log.txt").to_s

      @conn = Net::Telnet::new(
        'Hostname' => 'localhost',
        'Port'     => port,
        'Prompt'   => /./,
        'Waittime' => $TELNET_DELAY,
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
      "superSECURE!!!!passphrase123456789"
    end

    ###########################################################################
    def command(str)
      retval = nil
      IO.write @log_path, "\n$> #{str}\n", mode: "a"
      @conn.cmd(str) do |c|
        retval = ::Base::Terminal.unescape(c)
        @buffer << retval
        IO.write @log_path, retval, mode: "a"
      end
      return retval
    end

  end
end

