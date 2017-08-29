module Combat

  class Group

    attr_accessor :battle
    attr_reader :members
    attr_reader :opponents

    ###########################################################################
    def initialize
      @battle    = nil
      @members   = Set.new
      @opponents = Set.new
    end

    ###########################################################################
    def add_member(being)
      being.group.remove_member being if being.group
      @members.add being
      being.group = self
    end

    ###########################################################################
    def remove_member(being)
      @members.delete being
      @opponents.clear if @members.empty?
      being.group = nil
    end

    ###########################################################################
    def add_opponent(group)
      @opponents.add group
    end

    ###########################################################################
    def remove_opponent(group)
      @opponents.delete group
    end

    ###########################################################################
    def all_opponent_members
      @opponents.flat_map(:members)
    end

  end

end

