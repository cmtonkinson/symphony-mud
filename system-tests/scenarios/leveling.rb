module SystemTest

  define :tnl do
    from :create_admin
    with :admin
    with :vork

    test do
      # Initial TNL should be 100.
      vork 'summary'
      expect(/tnl:\s+100\D/im)

      # TNL to reach hero should be crazy high.
      admin 'promote vork 99'
      vork 'summary'
      expect(/tnl:\s+1000000\D/im)

      # Upon reaching hero, TNL should reset to 0.
      admin 'promote vork 100'
      vork 'summary'
      expect(/tnl:\s+0\D/im)

    end

  end

  define :default_gains do
    from :create_admin
    with :admin
    with :vork, { klass: 'warrior' }
    with :zaboo, { klass: 'mage' }

    before do
      admin 'promote vork 100'
      admin 'promote zaboo 100'
    end

    test do
      hp_regexp = /\d+\/(\d+)hp/im 
      matches = vork_client.buffer[-1].scan hp_regexp
      warrior_hp = matches[-1][0].to_i

      matches = zaboo_client.buffer[-1].scan hp_regexp
      mage_hp = matches[-1][0].to_i
      fail "Warrior should have more health than mage" unless warrior_hp > mage_hp


      mana_regexp = /\d+\/(\d+)mana/im 
      matches = vork_client.buffer[-1].scan mana_regexp
      warrior_mana = matches[-1][0].to_i

      matches = zaboo_client.buffer[-1].scan mana_regexp
      mage_mana = matches[-1][0].to_i
      fail "Mage should have more mana than warrior" unless mage_mana > warrior_mana
    end
  end

end

