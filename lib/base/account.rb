module Base

  class Account

    ###########################################################################
    def self.hash(message)
      binary = OpenSSL::PKCS5.pbkdf2_hmac(
        message,
        Base.configuration.hash_salt,
        Base.configuration.hash_iterations,
        Base.configuration.hash_digest.length,
        Base.configuration.hash_digest,
      )
      hex = binary.unpack("h*")
      hex.first # unpack returns an Array; in this case only a single element
    end

    ###########################################################################
    # Whew, didn't even have to work for it! Graciously accepted from the
    # precious armory of ruby-doc.org (OpenSSL::PKCS5 module docs)
    def self.equal_time_compare(left, right)
      # Don't continue if the lengths are different.
      return false unless left.length == right.length
      # Convert the right hand side to a byte array.
      right_bytes = right.bytes.to_a
      # We need an integer for bookkeeping.
      memo = 0
      # Compare each left byte with the corresponding right byte.
      left.bytes.each_with_index do |byte, index|
        # Memo will be non-zero if any of the bits diverge.
        memo |= byte ^ right_bytes[index]
      end
      # If memo is still zero, the two strings are the same.
      memo == 0
    end

    ###########################################################################
    def self.add(name, id, given_passphrase)
      user_map[name] = {
        id: id,
        passphrase: hash(given_passphrase)
      }
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
      if equal_time_compare user_map[name][:passphrase], hash(given_passphrase)
        user_map[name][:id]
      else
        false
      end
    end

    private

    ###########################################################################
    def self.file_path
      Symphony.data.join "accounts.yml"
    end

    ###########################################################################
    def self.user_map
      # Cache the file contents.
      @user_map ||= (YAML.load(File.read(file_path)) || {})
    end

    ###########################################################################
    def self.update_user_map!
      # Don't kill the file.
      raise ArgumentError.new "user_map is nil" if user_map.nil?
      # Write the updated YAML.
      File.open(file_path, "w") { |f| f.write YAML.dump(@user_map) }
      # Invalidate the cache.
      @user_map = nil
    end

  end

end
