require "securerandom"

module World

  class Objekt
    attr_reader :id
    attr_accessor :name

    ###########################################################################
    def initialize
      @id   = SecureRandom.uuid
      @name = "_name"
    end

    ###########################################################################
    # Given an obj of type Thing
    # Attempts to send `@things << obj`
    def insert(obj)
      container = obj.class.name.downcase.to_s + "s"
      raise ArgumentError.new "Cannot insert object of type #{obj.class.name}" unless respond_to? container
      send(container) << obj
    end
    alias_method :<<, :insert

    ###########################################################################
    # requires the definiton of the following in the child class:
    #
    def save_file_path
      klass  = self.class.name.split("::").last.downcase
      subdir = Symphony.data.join klass
      subdir.join "#{id}.yml"
    end

    ###########################################################################
    #
    def save
      File.open save_file_path, "w" do |f|
        f.write YAML.dump self
      end
    end

  end

end
