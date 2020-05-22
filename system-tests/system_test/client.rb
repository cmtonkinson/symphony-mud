require_relative '../../lib/base/terminal'

module SystemTest
  class Client

    ###########################################################################
    def initialize(port, state_path, account)
      @account = account
      @log_path = state_path.join("#{@account}-log.txt").to_s
      @conn = Net::Telnet::new(
        'Hostname' => 'localhost',
        'Port'     => port,
        'Prompt'   => / ((name|passphrase):) |
                        \[\d+\/\d+hp\s\d+\/\d+mana\s\d+staminas\d+tnl\] |
                        \z/ix,
      )
      sleep $TELNET_DELAY
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
    def login
      command @account
      command password
    end

    ###########################################################################
    def command(str)
      retval = nil
      IO.write @log_path, "\n$> #{str}\n", mode: "a"
      @conn.cmd(str) do |c|
        retval = c
        IO.write @log_path, ::Base::Terminal.unescape(retval), mode: "a"
      end
      sleep $TELNET_DELAY
      return retval
    end

  end
end

