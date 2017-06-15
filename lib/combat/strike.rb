module Combat

  class Strike

    attr_reader :attacker, :defender
    attr_reader :bout, :battle

    def initialize(attacker, defender, bout = nil)
      @attacker = attacker
      @defender = defender
      @bout = bout || Bout.new(attacker, defender)
      Symphony.logger.warn "strike"
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

      $world.schedule Mechanics::Job.new 1, Strike, :new, defender, attacker
    end
  end

end

