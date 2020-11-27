module SystemTest
  #############################################################################
  define :combat_setup do
    with :admin
    with :vork, { klass: 'warrior', gender: 'male', height: 'tall', weight: 'thin' }
    with :zaboo, { klass: 'mage', gender: 'male', height: 'short', weight: 'average' }
    test do
      vork 'preferences numeric on'
      zaboo 'preferences numeric on'

      # rid 1 - flaming sword
      admin 'iedit create'
      admin 'name flaming sword'
      admin 'wearable hold'
      admin 'type weapon'
      admin 'weapon_type sword'
      admin 'damage_type slash'
      admin 'hitroll 100'
      admin 'damroll 100'
      # TODO: allow assignment of weapon (Item?) element
      admin 'say do stuff from TODO above'
      admin 'information'
      admin 'exit'

      # rid 2 - fire pendant that offers flame absorption
      admin 'iedit create'
      admin 'name fire pendant'
      admin 'wearable neck'
      admin 'type jewelry'
      admin 'modify fire -150'
      admin 'information'
      admin 'exit'
    end
  end

  #############################################################################
  define :test_kill do
    from :combat_setup
    with :admin
    with :vork
    with :zaboo
    test do
      vork 'kill zaboo'
      sleep 2
      vork 'say flush buffer'
    end
  end

  #############################################################################
  define :elemental_absorb do
    from :combat_setup
    with :admin
    with :vork
    with :zaboo

    before do
      # Give Vork a flaming sword
      admin 'load item 1'
      admin 'drop sword'
      vork 'get sword'
      vork 'wear sword'

      # Make Zaboo fire absorbant
      admin 'load item 2'
      admin 'drop pendant'
      zaboo 'get pendant'
      zaboo 'wear pendant'
    end

    test do
      vork 'equipment'
      vork 'details'

      zaboo 'equipment'
      zaboo 'details'
      expect(/fire:\s+50% absorb/i)

      vork 'kill zaboo'
      sleep 2
      vork 'say flush buffer'

      # Without fire absorption, a level 1 Avatar being hit by a weapon with
      # hit/dam of 100/100 should be an insta-kill.
      n_expect_any zaboo, /DIED|KILLED/
    end
  end
end
