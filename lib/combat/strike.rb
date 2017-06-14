module Combat

  class Strike

    attr_reader :attacker, :defender
    attr_reader :bout, :battle

    def initialize(attacker, defender)
      Symphony.logger.debug "attacker %s defender %s" % [attacker,defender].map(&:fqid_str)
      @attacker = attacker
      @defender = defender
      exec
    end

    def exec
      attacker.stamina.decrease 1
      defender.stamina.decrease 2
      defender.health.decrease attacker.level / defender.level
      Symphony.logger.warn "Defender health left: %d/%d" % [defender.health.current, defender.health.max]
      attacker.puts "You strike $being!", defender
      defender.puts "$being strikes you!", attacker
      attacker.room.puts "$being strikes $being!", attacker, defender, exclude: [attacker, defender]
    end
  end

end
