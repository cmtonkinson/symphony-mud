# Monkeypatch the STDLIB Logger class with an additional verbosity
# level under DEBUG called TRACE.
require 'logger'

class Logger
  send :remove_const, 'SEV_LABEL'
  SEV_LABEL = {
    -1 => 'TRACE',
    0 => 'DEBUG',
    1 => 'INFO',
    2 => 'WARN',
    3 => 'ERROR',
    4 => 'FATAL',
    5 => 'ANY'
  }.freeze

  module Severity
    TRACE = -1
  end

  def trace(progname = nil, &block)
    add(TRACE, nil, progname, &block)
  end

  def trace?
    @level <= TRACE
  end
end

## Set this to STDOUT by default - it may change later.
Symphony.logger = Logger.new(STDOUT)

Symphony.logger.formatter = proc do |severity, datetime, _progname, msg|
  dev = Symphony.logger.instance_variable_get('@logdev')

  raw_msg = case msg
            when String then msg.clone
            when Exception then ([msg.class.name, msg.message] + msg.backtrace).join("\n  ")
            else msg.inspect
            end
  formatted_msg = Base::Terminal.uncolor raw_msg

  if dev.dev.is_a? File
    dev.write "[%s %d] %s -- %s: %s\n" % [
      datetime,
      Process.pid,
      severity,
      BINARY_NAME,
      formatted_msg
    ]
  else
    colors = {
      'TRACE' => '{w',
      'DEBUG' => '{x',
      'INFO' => '{x',
      'WARN' => '{y',
      'ERROR' => '{r',
      'FATAL' => '{R'
    }

    dev.write Base::Terminal.colorize("%s[%s %d] %s -- %s: %s{x\n" % [
      colors.key?(severity) ? colors[severity] : '',
      datetime,
      Process.pid,
      severity,
      BINARY_NAME,
      formatted_msg
    ])
  end

  next
end
