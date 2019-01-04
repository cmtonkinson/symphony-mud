module Util
  class Registry
    @@hash = {}

    ###########################################################################
    def self.hash
      @@hash
    end

    ###########################################################################
    def self.register(objekt)
      hash[objekt.class] ||= {}
      hash[objekt.class][objekt.id] = objekt
    end
    singleton_class.send :alias_method, :<<, :register

    ###########################################################################
    def self.unregister(objekt)
      hash[objekt.class].delete objekt.id
      hash.delete_if { |_klass, bucket| bucket.empty? }
    end
    singleton_class.send :alias_method, :-, :unregister

    ###########################################################################
    def self.lookup(objekt_id)
      objekt = nil
      if (bucket = hash.values.find { |bucket| bucket.key? objekt_id })
        objekt = bucket[objekt_id]
        if objekt._disposal_requested
          Symphony.logger.error "Lookup on disposed Objekt #{objekt.class.name} #{objekt.id}"
        end
      end

      objekt
    end
    singleton_class.send :alias_method, :[], :lookup

    ###########################################################################
    def self.report
      buckets = Hash[hash.map do |klass, bucket|
        [klass.name, {
          size: bucket.length
        }]
      end]
      {
        size: buckets.values.map { |b| b[:size] }.reduce(:+),
        buckets: buckets,
      }
    end

    ###########################################################################
    # Remove any disposed Objekts from the Registry. If all went well, the
    # registry should be the last reference to anything marked for disposal,
    # so after this the Ruby GC can actually reap them.
    def self.cleanup
      hash.each do |_klass, bucket|
        bucket.delete_if do |_objekt_id, objekt|
          Symphony.logger.trace "Disposing #{objekt.fqid_str}" if objekt.disposed?
          objekt.disposed?
        end
      end
      hash.delete_if { |_klass, bucket| bucket.empty? }
    end
  end
end
