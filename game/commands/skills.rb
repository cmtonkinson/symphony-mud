module Command
###############################################################################
define :kick do
###############################################################################
  usage
  usage({ victim: :id })
  is_ability
  exec do
    if being.battle
      target = being.acquire_target
      being.puts t('err_noone') and next if target.nil?
    elsif victim
      targets = room.beings.locate victim
      being.puts t('err_noone') and next if targets.empty?
      being.puts t('err_too_many') and next if targets.length > 1
      target = targets.first
    else
      being.puts t('err_no_target') and next
    end
    ability = being.get_ability :kick
    ability.invoke target
  end
end

end
