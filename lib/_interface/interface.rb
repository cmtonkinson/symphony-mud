module Interface

  def method(name)
    define_method(name) do |*args|
      raise "Interface method #{name} not implemented"
    end
  end

end

