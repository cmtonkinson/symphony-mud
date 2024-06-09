module Scripting
  class Error < StandardError
  end

  class ParsingError < Error
  end

  class ArgumentError < Error
  end

  class ReturnTypeError < Error
  end

  class RuntimeError < Error
  end

  class TimeoutError < Error
  end
end
