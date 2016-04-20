module Base

  class Account

    ###########################################################################
    def self.hash(message)
      OpenSSL::PKCS5.pbkdf2_hmac(
        message,
        Base.configuration.hash_salt,
        Base.configuration.hash_iterations,
        Base.configuration.hash_digest.length,
        Base.configuration.hash_digest,
      )
    end

    ###########################################################################
    def self.add(name, given_passphrase)
      user_map[name] = hash(given_passphrase)
      update_user_map!
    end

    ###########################################################################
    def self.remove(name)
      user_map.delete name
      update_user_map!
    end

    ###########################################################################
    def self.exists?(name)
      user_map.key? name
    end

    ###########################################################################
    def self.authenticate(name, given_passphrase)
      user_map[name] == hash(given_passphrase)
    end

    ###########################################################################
    def self.load(name)
      user_map[name]
    end

    private

    ###########################################################################
    def self.file_path
      Symphony.data.join("accounts.yml")
    end

    ###########################################################################
    def self.user_map
      # Cache the file contents.
      @user_map ||= YAML.load File.read file_path
    end

    ###########################################################################
    def self.update_user_map!
      # Don't kill the file.
      raise ArgumentError.new "user_map is nil" if user_map.nil?
      # Write the updated YAML.
      File.open(file_path, "w") { |f| f.write @user_map }
      # Invalidate the cache.
      @user_map = nil
    end

  end

end
