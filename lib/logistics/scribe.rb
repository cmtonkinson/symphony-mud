module Logistics
  class Scribe
    ###########################################################################
    def self.save(objekt)
      path    = pathname objekt.class, objekt.id
      content = objekt.serialize
      # Ensure the directory exists.
      FileUtils.mkdir_p File.dirname path
      # Write the serialized content to disk.
      File.open(path, 'w') { |fp| fp.write content }
    end

    ###########################################################################
    def self.load(klass, objekt_id)
      File.open pathname(klass, objekt_id), 'r' do |fp|
        begin
          klass.deserialize fp.read
        rescue NoMethodError => e
          Symphony.logger.error "Error while loading #{klass}:#{objekt_id}"
          Symphony.logger.debug "  error at #{e.backtrace[0]}"
          Symphony.logger.debug "  error at #{e.backtrace[1]}"
          Symphony.logger.debug "  error at #{e.backtrace[2]}"
          return false
        end
      end
    end

    ###########################################################################
    def self.pathname(klass, objekt_id)
      dirname(klass).join "#{objekt_id}.yml"
    end

    ###########################################################################
    def self.dirname(klass)
      Symphony.data.join klass.name.split('::').last.downcase
    end

    ###########################################################################
    # Returns all zone ids found in the data directory.
    def self.the_story_of_baron_munchausen
      Dir[Symphony.zone.join '*.yml'].map do |zonefile|
        File.basename(zonefile).gsub /\.yml/, ''
      end
    end
  end
end
