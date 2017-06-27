module Combat

  def self.embattle(attacker, defender)
    belligerents = [attacker, defender]

    # Both belligerents are already in combat.
    # Not much work to do here.
    if belligerents.all? &:in_battle?
      raise "attacker and defender in different battles" if attacker.circumstance != defender.circumstance

    # One of the two belligerents is already in combat.
    # Bring the other one into the battle.
    elsif attacker.in_battle? || defender.in_battle?
      already_fighting = belligerents.find &:in_battle?
      not_yet_fighting = belligerents.find { |b| !b.in_battle? }
      already_fighting.circumstance.add_belligerent not_yet_fighting

    # Neither party is yet in battle. We need to fix that :)
    else
      battle = Battle.new
      battle.add_belligerent attacker
      battle.add_belligerent defender 
    end
  end

end

