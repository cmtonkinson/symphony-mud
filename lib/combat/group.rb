module Combat
  class Group
    attr_accessor :battle
    attr_reader :leader
    attr_reader :members
    attr_reader :pending
    attr_reader :opponents

    ###########################################################################
    def initialize(leader)
      @battle    = nil
      @leader    = nil
      @members   = Array.new
      @opponents = Array.new
      @pending   = Array.new

      add_member leader
      @leader = leader
    end

    ###########################################################################
    def add_member(being)
      raise ArgumentError.new "Cannot add a #{being.class.name} as a Group member" unless being.is_a? Realm::Being

      being.group.remove_member being if being.group
      @members.push being
      being.set_group self
    end

    ###########################################################################
    def remove_member(being, replace = true)
      @members.delete being
      @opponents.clear if @members.empty?

      being.set_group nil
      Combat::Group.new being if replace
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
    def remove_all_opponents
      @opponents = Array.new
    end

    ###########################################################################
    def puts(format, *args)
      @members.each { |m| m.puts(format, *args) }
    end

    ###########################################################################
    def leave(being)
      remove_member being
      being.puts 'You have left the group led by $being.', leader
      puts '$being has left the group.', being
    end

    ###########################################################################
    def disband
      puts 'The group has been disbanded.'
      @members.each { |m| remove_member m }
      @battle    = nil
      @leader    = nil
      @opponents = nil
      @pending   = nil
      @leader    = nil
    end

    ###########################################################################
    def request_join(being)
      return if being.pending_group == self

      being.pending_group.request_withdraw being if !being.pending_group.nil?

      @pending.push being
      being.pending_group = self
      leader.puts '$being has requested to join your group.', being
      being.puts 'You have requested to join the group led by $being.', leader
    end

    ###########################################################################
    def request_withdraw(being)
      return unless @pending.include? being

      @pending.delete being
      being.pending_group = nil
      being.puts 'You have withdrawn your request to join the group led by $being.', leader
      leader.puts '$being has withdrawn $possessive group membership request.', being, being
    end

    ###########################################################################
    def request_accept(being)
      return unless @pending.include? being

      being.pending_group = nil
      @pending.delete being
      being.puts '$being has accepted your group membership.', leader
      puts '$being has joined the group.', being
      add_member being
    end

    ###########################################################################
    def request_deny(being)
      return unless @pending.include? being

      @pending.delete being
      being.pending_group = nil
      being.puts '$being has declined your group membership request.', leader
      leader.puts 'You have denied $being membership into your group.', being
    end

    ###########################################################################
    # Returns true if there is at least one Group member who can acquire a
    # valid target.
    def acquire_targets
      members.any? { |m| m.acquire_target }
    end

    ###########################################################################
    # NOTE: This needs to be changed so that the experience awarded to the
    # Group is distributed across the members in some interesting way, rather
    # than just dividing it evenly.
    def award_experience(exp_gained, stiff)
      eligible = members.reject { |m| m.room != stiff.room }.reject { |m| m.dead? }
      eligible.each { |m| m.award_experience exp_gained / eligible.size }
    end
  end
end
