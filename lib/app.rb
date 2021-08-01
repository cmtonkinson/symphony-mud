require 'ostruct'

###############################################################################
# If you're familiar with Rails, a lot of the patterns for this App class will
# be familiar to you.
#
# @root is a Pathname to the root project directory.
#
# This class is designed to be a globally-available singleton.
###############################################################################
class App
  KONSTANTS = 'konst.rb'.freeze

  DIRECTORIES = {
    boot: %i[setup],
    commands: %i[],
    lib: %i[],
    data: %i[avatar pidfile zone],
    game: %i[],
    help: %i[],
    locales: %i[],
    templates: %i[],
    tmp: %i[]
  }.freeze

  # Create a reader accessor for each project directory.
  DIRECTORIES.each do |dir, subdirs|
    attr_reader dir

    subdirs.each do |subdir|
      attr_reader subdir
    end
  end

  # Other readers and accessors.
  attr_reader :root, :accounts, :binary, :configuration
  attr_accessor :logger

  #############################################################################
  def initialize
    @root          = Pathname.new File.expand_path '..', __dir__
    @binary        = @root.join BINARY_NAME
    @logger        = nil
    @configuration = OpenStruct.new

    DIRECTORIES.each do |dir, subdirs|
      instance_variable_set "@#{dir}", @root.join(dir.to_s)
      subdirs.each do |subdir|
        instance_variable_set "@#{subdir}", @root.join(dir.to_s, subdir.to_s)
      end
    end
  end

  #############################################################################
  # This method primarily exists to support the hack at the bottom of the
  # config/configuration.rb script, but is structured in a way that also could
  # allow for some tricky ops maneuvers such as snapshotting vs overwriting
  # worldsaves.
  def update_data_paths!
    dd = configuration.data_dir
    prefix = dd[0] == '/' ? Pathname.new(dd) : @root.join(dd)
    instance_variable_set '@data', prefix
    DIRECTORIES[:data].each do |subdir|
      instance_variable_set "@#{subdir}", prefix.join(subdir.to_s)
    end
  end
end
