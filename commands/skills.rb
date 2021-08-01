module Command
  ###############################################################################
  define :cast do
    usage({ spell: :string })
    usage({ spell: :string }, { target: :id })
    exec do
      # Locate the spell
      ability = being.get_ability spell
      being.puts t('err_no_spell') and next if ability.nil?

      # Locate the target
      if target.nil?
        case ability.default_target
        when :self
          victim = being
        when :enemy
          victim = being.acquire_target
          being.puts t('err_noone') and next if victim.nil?
        end
      else
        victims = room.beings.locate target
        being.puts t('err_noone') and next if victims.empty?
        being.puts t('err_too_many') and next if victims.length > 1

        victim = victims.first
      end

      # Cast it like you... are fishing?
      #                 ... broke it playing hockey?
      #                 ... own a foundry?
      #                 ... I don't know, I got nuthin, make your own jokes pal.
      being.cast_spell ability, victim
    end
  end
end
