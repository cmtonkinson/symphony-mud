module Realm
  #############################################################################
  def container_for_klass(klass)
    container_name = nil

    klass.ancestors.each do |ancestor|
      # If we've reached up the object heirarchy all the way to Objekt, then
      # there are no valid containers configured for this klass or any of
      # its ancestors.
      raise ArgumentError.new "No #{klass.name} container configured on #{self.class.name}" if ancestor == Realm::Objekt

      # Construct a container name from the ancestor.
      container_name = ancestor.name.split('::').last.downcase.to_s + 's'

      # If the Objekct responds to the container name, then we've found the
      # most-specific container configured to accept objects of this type.
      break if respond_to? container_name
    end

    send container_name
  end

  #############################################################################
  def next_objekt_rid(klass)
    container = container_for_klass klass
    return 1 if container.empty?

    container.values.max_by(&:rid).rid + 1
  end

  #############################################################################
  def find_by_rid(klass, rid)
    container = container_for_klass klass
    return nil if container.empty?

    container.values.find { |objekt| objekt.rid == rid }
  end
end
