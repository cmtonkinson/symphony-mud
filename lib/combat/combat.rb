module Combat

  def self.embattle(attacker, defender)
    battle = Battle.new
    attacker.circumstance = battle
    defender.circumstance = battle

    battle.add_belligerent attacker
    battle.add_belligerent defender    

    $world.schedule Mechanics::Job.new 1, Combat::Strike, :new, attacker, defender
  end

end
