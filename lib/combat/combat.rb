module Combat

  def self.embattle(attacker, defender)
    battle = attacker.group.battle || defender.group.battle || Battle.new
    battle.add_belligerent attacker.group
    battle.add_belligerent defender.group

    attacker.group.add_opponent defender.group
    defender.group.add_opponent attacker.group
  end

end

