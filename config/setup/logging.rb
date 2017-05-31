Symphony.logger = Logger.new(STDOUT)

Symphony.logger.formatter = proc do |severity, datetime, progname, msg|
  colors = {
    "DEBUG" => "{w",
    "INFO"  => "{x",
    "WARN"  => "{y",
    "ERROR" => "{r",
    "FATAL" => "{R",
  }
  
  puts Base::Terminal.colorize("%s[%s %d] %s -- %s: %s{x" % [
    colors.key?(severity) ? colors[severity] : "",
    datetime,
    Process.pid,
    severity,
    BINARY_NAME,
    Base::Terminal.uncolor(msg.is_a?(String) ? msg.clone : msg.inspect),
  ])
end

