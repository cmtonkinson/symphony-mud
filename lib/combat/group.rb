module Combat

  class Group

    attr_accessor :battle
    attr_reader :members
    attr_reader :opponents

    ###########################################################################
    def initialize(leader)
      @battle    = nil
      @members   = Array.new
      @opponents = Array.new

      add_member leader
    end

    ###########################################################################
    def add_member(being)
      raise ArgumentError.new "Cannot add a #{being.class.name} as a Group member" unless being.is_a? Realm::Being
      being.group.remove_member being if being.group
      @members.push being
      being.set_group self
    end

    ###########################################################################
    def remove_member(being)
      @members.delete being
      @opponents.clear if @members.empty?
      being.set_group nil
    end

    ###########################################################################
    def member_died(being)
      
    end

    ###########################################################################
    def add_opponent(group)
      raise ArgumentError.new "Cannot add a #{group.class.name} as a Group opponent" unless group.is_a? Group
      @opponents.push group
    end

    ###########################################################################
    def remove_opponent(group)
      @opponents.delete group
    end

    ###########################################################################
    def all_opponent_members
      @opponents.map { |b| b.members }.reduce(:+)
    end

    ###########################################################################
    # Returns true if there is at least one Group member who can acquire a
    # valid target.
    def acquire_targets
      members.any? { |m| m.acquire_target }
    end

  end

end

