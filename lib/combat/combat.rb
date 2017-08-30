module Combat

  def self.embattle(attacker, defender)
    # Reference an existing Battle if one exists, otherwise start a new one.
    battle = attacker.group.battle || defender.group.battle || Battle.new

    # Add each Group to the Battle.
    battle.add_belligerent attacker.group
    battle.add_belligerent defender.group

    # Make each Group aware of its opponents.
    attacker.group.add_opponent defender.group
    defender.group.add_opponent attacker.group
  end

end

