require "securerandom"

module World

  class Objekt
    attr_reader :id
    attr_accessor :name

    ###########################################################################
    def initialize(id = nil)
      @id   = id || SecureRandom.uuid
      @name = "_name"
    end

    ###########################################################################
    # Given an obj of type Thing
    # Attempts to send `@things << obj`
    def insert(obj)
      container = obj.class.name.split("::").last.downcase.to_s + "s"
      raise ArgumentError.new "Cannot insert object of type #{obj.class.name}" unless respond_to? container
      send(container) << obj
    end
    alias_method :<<, :insert

    ###########################################################################
    def save
      File.open self.class.file_path(id), "w" do |f|
        f.write serialize
      end
    end

    ###########################################################################
    def self.load(objekt_id)
      File.open file_path(objekt_id), "r" do |f|
        deserialize f.read
      end
    end

    ###########################################################################
    def serialize
      YAML.dump self
    end

    ###########################################################################
    def self.deserialize(yaml)
      YAML.load yaml
    end

    ###########################################################################
    def self.file_path(objekt_id)
      klass  = self.name.split("::").last.downcase
      subdir = Symphony.data.join klass
      subdir.join "#{objekt_id}.yml"
    end

  end

end
