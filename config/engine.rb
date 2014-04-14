class Engine
  attr_reader :root, :env

  def initialize
    @root = Pathname.new File.expand_path '../..', __FILE__
    @env  = ENV['SYMPHONY_ENV'].to_sym
  end

end
