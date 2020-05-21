# Set this to STDOUT by default - it may change later.
Symphony.logger = Logger.new(STDOUT)

Symphony.logger.formatter = proc do |severity, datetime, _progname, msg|
  dev = Symphony.logger.instance_variable_get("@logdev")
  
  if dev.dev.is_a? File
    dev.write "[%s %d] %s -- %s: %s\n" % [
      datetime,
      Process.pid,
      severity,
      BINARY_NAME,
      msg.is_a?(String) ? msg.clone : msg.inspect
    ]
  else
    colors = {
      'TRACE' => '{w',
      'DEBUG' => '{x',
      'INFO' => '{x',
      'WARN' => '{y',
      'ERROR' => '{r',
      'FATAL' => '{R',
    }

    dev.write Base::Terminal.colorize("%s[%s %d] %s -- %s: %s{x\n" % [
      colors.key?(severity) ? colors[severity] : '',
      datetime,
      Process.pid,
      severity,
      BINARY_NAME,
      Base::Terminal.uncolor(msg.is_a?(String) ? msg.clone : msg.inspect),
    ])
  end
end
