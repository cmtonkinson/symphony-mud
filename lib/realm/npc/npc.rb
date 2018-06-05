module Realm
  class NPC < Being
    # NPCs (during building) have an obvious need for full stat manipulation
    # even though Being says that's 'discouraged'.
    EXPERIENCE.each { |name| attr_writer name }
    WELLNESS.each { |name| attr_writer name }
    FITNESS.each { |name| attr_writer name }
    DEFENSE.each { |name| attr_writer name }
    OFFENSE.each { |name| attr_writer name }

    ###########################################################################
    def initialize(id = nil, props = {})
      super
    end

    ###########################################################################
    def _interpret_name
      '$being'
    end

    ###########################################################################
    def summary
      $templates.npc_summary % [
        rid, name,
        id,
        level,
        %w[].sort.join(', '),
      ]
    end
  end
end

