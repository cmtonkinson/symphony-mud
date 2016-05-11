module Logger

  class CommandLine < Logger

    def _log(message)
      puts Base::Terminal.colorize message
    end

  end

end
